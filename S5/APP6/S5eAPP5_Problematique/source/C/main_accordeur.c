/* accordeur.c */
/*
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   Paul Charette (automne 2016)
  	  	  	  - Chang� le nom de certains variables dans c_int11() pour rendre le
  	  	  	  	code plus compr�hensible.
  	  	  	  - Ajout� � c_int11() la capacit� de sortir les �chantillons
  	  	  	    non-filtr�s et filtr�s sur les canaux droite/gauche de OUT
  	  	  	    pour d�bugger les filtres ind�pendemment du calcul de l'erreur
  	  	  	    d'accordage.

  DESCRIPTION : 
     Code principal pour un accordeur de guitare. Permet d'�couter 
     la fr�quence fondamentale des cordes de guitare et d'obtenir 
     l'erreur sur l'accordement.

  ENTR�ES : 
	 Il y a deux types d'entr�es physiques :
     1) Signal audio branch� sur l'entr�e LINE IN du DSK. Ce signal
     correspondant � une note de guitare jou�e par un musicien. 
     2) Les s�lections de l'utilisateur provenant des boutons du DSK.

  SORTIES : 
     1) Erreur sur l'accordement de la note jou�e. Cette erreur
        est transmise sur le canal gauche de la sortie HEADPHONE du DSK.
	 2) Signal sinuso�dal correspondant � la fr�quence fondamentale
	    de la note s�lectionn�e. Ce signal est transmis sur le canal droit
		de la sortie HEADPHONE du DSK (Pour �couter il faut appuyer sur
		la DIP SWITCH #3)
*/
#include <stdio.h>
#include <math.h>
#include "main_accordeur.h"
#include "CONSTANTES.h"
#include "deltaAngle_CordesGuit.h"
#include "dsk6713_aic23.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "genCos.h"
#include "findErrAccordage.h"
#include "getNewCommand.h"
#include "C6713Helper_UdeS.h"


// VARIABLES GLOBALES
USELECTIONS Commandes;				// Commandes de l'utilisateur (voir main_accordeur.h)
const float F0_NOMINAL[NB_CORDES] = // Fr�quences fondamentales de chacune des cordes
     { 82.407,110.000,146.832,195.998,246.942,329.628 };
float tamponEchFilt[L_TAMPON];			// Tampon d'�chantillons
int noEchFilt=0;						// Num�ro de l'�chantillon courant
float errAccordement;				// Erreur sur l'accordement de l'instrument

// VARIABLES GLOBALES POUR DSK
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ; 			 // Fr�quence d'�chantillonnage
#define DSK6713_AIC23_INPUT_LINE 0x0011		 // D�finition de l'entr�e LINE IN
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // Selection de l'entr�e LINE IN

#define GAUCHE 0 // D�finition du haut-parleur gauche
#define DROIT  1 // D�finition du haut-parleur droit
union {Uint32 uint; short channel[2];} AIC23_data; // Pour contenir les deux signaux

void main() 
{
	afficherMenu();		// Affichage du menu principal � l'�cran
	initAccordeur();	// Initialisations des variables et du hardware

	while(1) {

		// Si le tampon d'�chantillons filtr�s pour l'autocorr�lation est plein
		if (noEchFilt == L_TAMPON) {
			// Calcul de l'erreur d'accordage
			errAccordement = findErrAccordage(tamponEchFilt, F0_NOMINAL[Commandes.noCorde-1]);

			// Attendre quelques instants pour permettre au tampon de bien se remplir
			attendre(0.1); // NOTE : N�cessaire pour le bon fonctionnement des interruptions

			// Permettre au tampon de se remplir de nouveau
			noEchFilt = 0;
		}
	}
}

// Attente en sec. (approximatif)
void attendre(float seconds)
{
	int cnt=0;
	int fin = (int)NB_CYCLES_PAR_SEC*seconds;
	while (cnt++<fin) {}
}

/********************************************************************************************
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Code execut� � toutes les interruptions
********************************************************************************************/
interrupt void c_int11() 
{
	float echOut; 		 // Amplitude de l'�chantillon g�n�r�e pour l'�coute d'une note
	short echLineIn;	 // Ampliutde de l'�chantillon provenant de l'entr�e LINE IN
	short echLineInFilt; // Ampliutde de l'�chantillon filtr�
	short pwm[10] = {0,0,0,0,0,0,0,0,25,25}; // Pulse width modulation
	int debugFiltres = 0;
	static int n = 0;

	// Capture de l'�chantillon provenant de l'entr�e "IN"
	echLineIn = (short) input_sample();

	//
	// VOTRE **SOLUTION** DE FILTRAGE FIR + IIR REMPLACE LA PROCHAINE LIGNE!!!
	echLineInFilt = echLineIn;
	//
	//

	// Si le tampon d'�chantillons filtr�s pour l'autocorr�lation n'est pas plein,
	// y ajouter l'�chantillon courant filtr�
	if (noEchFilt < L_TAMPON) {
		// Enregistrement de l'�chantillon dans le tampon
		tamponEchFilt[noEchFilt++] = (float) echLineInFilt;
	}

	// Si une �coute est demand�e, sortir l'amplitude sur le canal droit
	if (Commandes.isPlay) {
		// G�n�rer l'�chantillon audio pour �couter la tonalit� de la corde
		echOut = genFrqCos(Commandes.noCorde, Commandes.isPlay);

		// VOTRE **SOLUTION**, � VOUS DE CODER CETTE FONCTION
//		echOut = genFrq_methode2(Commandes.noCorde, Commandes.isPlay);	

		// Assigner l'�chantillon au canal droit de la sortie HEADPHONE
		AIC23_data.channel[DROIT] =  (short)(1000*echOut);
	}
	// Autrement, sortir l'�chantillon d'entr�e non-filtr�e sur le canal droit
	// pour fins de d�buggage de vos filtres
	else
		AIC23_data.channel[DROIT] = echLineIn;

	// Assigner l'erreur d'accordement au canal gauche de la sortie HEADPHONE
	// NOTE : Branchez le canal gauche � un oscilloscope pour observer l'erreur
	if (debugFiltres == 0)
		AIC23_data.channel[GAUCHE] = (short)errAccordement*pwm[n++%10];
	// Autrement, sortir l'�chantillon filtr� sur le canal gauche,
	// pour fins de d�buggage de vos filtres
	else
		AIC23_data.channel[GAUCHE] = echLineInFilt;

	// Sortir les deux signaux sur "HP/OUT"
	output_sample(AIC23_data.uint);			

	// Capturer des commandes usager provenant des DIP switchs
	getCommandes(&Commandes); 

	// Afficher les commandes entr�es par l'utilisateur sur les LEDs
	ajusterLED(Commandes.noCorde);

	return;
}

/********************************************************************************************
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Code pour faire toutes les initialisations mat�riels
********************************************************************************************/
void initAccordeur() 
{
	// INITIALISATION DES VARIABLES
	Commandes.noCorde = 1;		// La corde #1 est par d�fault la premi�re corde � accorder
	Commandes.isPlay = 0;		// Initialement, n'�coute pas la note
	

	// INITIALISATION DU HARDWARE
	DSK6713_DIP_init();	// Initialisation de la communication du DSP avec les 4 DIP swichs 
	DSK6713_LED_init();	// Initialisation des 4 LEDS (�teindre)

	comm_intr();		/* Initialisation de la communication entre DSP et AIC23 codec
						   et d�marage des interruptions pour l'�chantillonnage */

	return;			
}

/********************************************************************************************
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Code pour afficher l'information pour l'usager � l'�cran
********************************************************************************************/
void afficherMenu()
{
		printf ("Menu :");
		printf ("\n DIP 0 : Corde inf�rieure");
		printf ("\n DIP 1 : Corde sup�rieure"); 
		printf ("\n DIP 2 : ---");
		printf ("\n DIP 3 : �couter la note de la corde");
		printf ("\n \n NOTE : Le num�ro de la corde s�lectionn�e est affich� en binaire sur les LEDs. \n");

	return;
}

/********************************************************************************************
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Permet d'afficher en binaire le num�ro de la corde s�lectionn�e sur les LED 
	ENTR�E : 
	 noCorde : Num�ro de la corde de guitare s�lectionn�e
********************************************************************************************/
void ajusterLED(int noCorde) 
{
	int ledNo, base;

	// Ajustement du num�ro de la corde sur les trois premi�res LED
	for (ledNo = 2; ledNo>=0; ledNo--) { // Pour chacun des 3 LEDS
		base = (int) pow(2,ledNo);		// Alg�bre binaire pour trouver les LEDs � allumer
		if ( base <= noCorde)			
		{
			noCorde = noCorde-base; 
			DSK6713_LED_on(ledNo);
		}
		else DSK6713_LED_off(ledNo); 	// Eteint la LED associ� au bit
	}

	return;
}

/********************************************************************************************
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Permet de faire l'acquisition des commandes utilisateurs.
     Le code contient un algorithme d'anti-rebond.
********************************************************************************************/
int getCommandes() 
{
	int isNew;		// Sortie de la fonction (flag : 1 si une nouvelle commande)
	int noDIP;		// Num�ro de la DIP appuy�

	isNew = getNewCommand(&noDIP); // flag : 1 si nouvelle commande (algo anti-rebond)

	// Segment de code pour le choix de la corde 
	if(isNew)	// si une nouvelle commande

		switch (noDIP) {
			case 0 : // si la commande est la DIP #0
				Commandes.noCorde = max(Commandes.noCorde-1,1); // La nouvelle corde est la corde inf�rieure
				isNew = 1;
			break;
			case 1 : // si la commande est la DIP #1
				Commandes.noCorde = min(Commandes.noCorde+1,NB_CORDES); // La nouvelle corde est la corde sup�rieure
				isNew = 1;
			break;
			case 2 : // si la commande est la DIP #2
			break;
			case 3 : // si la commande est la DIP #3
			break;
			default :
			break;
		}

	else {}		// L'usager n'a pas fait une nouvelle commande

	// Segment de code pour l'�coute d'une note
	if (DSK6713_DIP_get(3) == 0)  // S'il y a une �coute
		Commandes.isPlay = 1;
	else
		Commandes.isPlay = 0;

	return isNew;
}

// pour obtenir le minimum
int min(int a, int b) 
{
	if (a<b)	return a;
	else		return b; 
}

// pour obtenir le maximum
int max(int a, int b) 
{
	if (a>b)	return a;
	else		return b; 
}

/********************************************************************************************
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Permet d'�couter la fr�quence fondamentale de la note de la corde.
	ENTR�E : 
	 noCorde : Num�ro de la corde de guitare s�lectionn�e
	 isPlay : Flag (1 si une fr�quence fondamentale est pr�sentement g�n�r�e)
    RETOUR : 
	 �chantillon de sortie
	NOTE : 
	 M�thode avec la fonction cos() de la librairie standard <math.h>
********************************************************************************************/
float  genFrqCos(int noCorde, int isPlay) 
{
	float echAmp; 						// Amplitude de l'�chantillon
	static int memIsPlay = 0;			// M�moire de isPlay
	static float angle;					// Angle

	if (isPlay == 1 && memIsPlay == 0) 	 // Si c'est le d�but d'une �coute de note
		angle = 0;  					 // Initialisation de l'angle
	else	{}

	echAmp = genCos(DeltaAngle[noCorde-1], &angle);
	memIsPlay = isPlay;

	return echAmp;
}

/********************************************************************************************
  Cr�ateur:    
  Date:        
  Revisions: 
 DESCRIPTION : Permet d'�couter la fr�quence fondamentale de la note de la corde 
 NOTE : Vous devez choisir une m�thode et l'impl�menter
********************************************************************************************/
float  genFrq_methode2(int noCorde, int isPlay) 
{
	// �CRIVEZ VOTRE CODE ICI 
	// NOTE : Vous devez cr�er un .c contenant � la fois les fonctions
	// genM2(?) et initGenM2(?) (fct. de g�n�ration et fct. d'initialisation)
	// Vous devez aussi cr�er un fichier .dat
	// Insp�rez-vous de la fonction genFrq_cos(...)
	/*==========================================================================*/
	return 0;

}

