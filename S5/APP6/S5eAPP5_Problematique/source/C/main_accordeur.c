/* accordeur.c */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   Paul Charette (automne 2016)
  	  	  	  - Changé le nom de certains variables dans c_int11() pour rendre le
  	  	  	  	code plus compréhensible.
  	  	  	  - Ajouté à c_int11() la capacité de sortir les échantillons
  	  	  	    non-filtrés et filtrés sur les canaux droite/gauche de OUT
  	  	  	    pour débugger les filtres indépendemment du calcul de l'erreur
  	  	  	    d'accordage.

  DESCRIPTION : 
     Code principal pour un accordeur de guitare. Permet d'écouter 
     la fréquence fondamentale des cordes de guitare et d'obtenir 
     l'erreur sur l'accordement.

  ENTRÉES : 
	 Il y a deux types d'entrées physiques :
     1) Signal audio branché sur l'entrée LINE IN du DSK. Ce signal
     correspondant à une note de guitare jouée par un musicien. 
     2) Les sélections de l'utilisateur provenant des boutons du DSK.

  SORTIES : 
     1) Erreur sur l'accordement de la note jouée. Cette erreur
        est transmise sur le canal gauche de la sortie HEADPHONE du DSK.
	 2) Signal sinusoïdal correspondant à la fréquence fondamentale
	    de la note sélectionnée. Ce signal est transmis sur le canal droit
		de la sortie HEADPHONE du DSK (Pour écouter il faut appuyer sur
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
#include "genCosM2.h"
#include "findErrAccordage.h"
#include "getNewCommand.h"
#include "C6713Helper_UdeS.h"


// VARIABLES GLOBALES
USELECTIONS Commandes;				// Commandes de l'utilisateur (voir main_accordeur.h)
const float F0_NOMINAL[NB_CORDES] = // Fréquences fondamentales de chacune des cordes
     { 82.407,110.000,146.832,195.998,246.942,329.628 };
float tamponEchFilt[L_TAMPON];			// Tampon d'échantillons
int noEchFilt=0;						// Numéro de l'échantillon courant
float errAccordement;	

// Variable globale pour la génération de signal
const int fe = 8000;
const float PI = 3.14159265358979;
extern struct complx C_delta[NB_CORDES];
extern int nb[NB_CORDES];			// Erreur sur l'accordement de l'instrument

#define TAMPON_L  256
#pragma DATA_ALIGN(tampon, TAMPON_L*2); // Requis pour l'adressage circulaire en assembleur
short tampon[TAMPON_L]={0};         // Tampon d'échantillons
short *pTampon=&tampon[TAMPON_L-1]; // Pointeur sur l'échantillon courant

// VARIABLES GLOBALES POUR DSK
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ; 			 // Fréquence d'échantillonnage
#define DSK6713_AIC23_INPUT_LINE 0x0011		 // Définition de l'entrée LINE IN
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // Selection de l'entrée LINE IN

#define GAUCHE 0 // Définition du haut-parleur gauche
#define DROIT  1 // Définition du haut-parleur droit
union {Uint32 uint; short channel[2];} AIC23_data; // Pour contenir les deux signaux

void main()
{
	initGenM2();
	afficherMenu();		// Affichage du menu principal à l'écran
	initAccordeur();	// Initialisations des variables et du hardware

	while(1) {

		// Si le tampon d'échantillons filtrés pour l'autocorrélation est plein
		if (noEchFilt == L_TAMPON) {
			// Calcul de l'erreur d'accordage
			errAccordement = findErrAccordage(tamponEchFilt, F0_NOMINAL[Commandes.noCorde-1]);

			// Attendre quelques instants pour permettre au tampon de bien se remplir
			attendre(0.1); // NOTE : Nécessaire pour le bon fonctionnement des interruptions

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
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Code executé à toutes les interruptions
********************************************************************************************/
interrupt void c_int11() 
{
	float echOut; 		 // Amplitude de l'échantillon générée pour l'écoute d'une note
	short echLineIn;	 // Amplitude de l'échantillon provenant de l'entrée LINE IN
	short echLineInFilt; // Amplitude de l'échantillon filtré
	short pwm[10] = {0,0,0,0,0,0,0,0,25,25}; // Pulse width modulation
	int debugFiltres = 1;
	static int n = 0;

	// Capture de l'échantillon provenant de l'entrée "IN"
	echLineIn = (short) input_sample();

	//
	// VOTRE **SOLUTION** DE FILTRAGE FIR + IIR REMPLACE LA PROCHAINE LIGNE!!!
	//echLineInFilt = echLineIn;
	pTampon = FIR_ASM(pTampon, echLineIn, coef, &echLineInFilt);
	echLineInFilt = filtrerCascadeIIR(Commandes.noCorde, echLineInFilt);
	//
	//

	// Si le tampon d'échantillons filtrés pour l'autocorrélation n'est pas plein,
	// y ajouter l'échantillon courant filtré
	if (noEchFilt < L_TAMPON) {
		// Enregistrement de l'échantillon dans le tampon
		tamponEchFilt[noEchFilt++] = (float) echLineInFilt;
	}

	// Si une écoute est demandée, sortir l'amplitude sur le canal droit
	if (Commandes.isPlay) {
		// Générer l'échantillon audio pour écouter la tonalité de la corde
		//echOut = genFrqCos(Commandes.noCorde, Commandes.isPlay);

		// VOTRE **SOLUTION**, À VOUS DE CODER CETTE FONCTION
		echOut = genFrq_methode2(Commandes.noCorde, Commandes.isPlay);	

		// Assigner l'échantillon au canal droit de la sortie HEADPHONE
		AIC23_data.channel[DROIT] =  (short)(1000*echOut);
	}
	// Autrement, sortir l'échantillon d'entrée non-filtrée sur le canal droit
	// pour fins de débuggage de vos filtres
	else
		AIC23_data.channel[DROIT] = echLineIn;

	// Assigner l'erreur d'accordement au canal gauche de la sortie HEADPHONE
	// NOTE : Branchez le canal gauche à un oscilloscope pour observer l'erreur
	if (debugFiltres == 0)
		AIC23_data.channel[GAUCHE] = (short)errAccordement*pwm[n++%10];
	// Autrement, sortir l'échantillon filtré sur le canal gauche,
	// pour fins de débuggage de vos filtres
	else
		AIC23_data.channel[GAUCHE] = echLineInFilt;

	// Sortir les deux signaux sur "HP/OUT"
	output_sample(AIC23_data.uint);			

	// Capturer des commandes usager provenant des DIP switchs
	getCommandes(&Commandes); 

	// Afficher les commandes entrées par l'utilisateur sur les LEDs
	ajusterLED(Commandes.noCorde);

	return;
}

/********************************************************************************************
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Code pour faire toutes les initialisations matériels
********************************************************************************************/
void initAccordeur() 
{
	// INITIALISATION DES VARIABLES
	Commandes.noCorde = 1;		// La corde #1 est par défault la première corde à accorder
	Commandes.isPlay = 0;		// Initialement, n'écoute pas la note
	

	// INITIALISATION DU HARDWARE
	DSK6713_DIP_init();	// Initialisation de la communication du DSP avec les 4 DIP swichs 
	DSK6713_LED_init();	// Initialisation des 4 LEDS (éteindre)

	comm_intr();		/* Initialisation de la communication entre DSP et AIC23 codec
						   et démarage des interruptions pour l'échantillonnage */

	return;			
}

/********************************************************************************************
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Code pour afficher l'information pour l'usager à l'écran
********************************************************************************************/
void afficherMenu()
{
		printf ("Menu :");
		printf ("\n DIP 0 : Corde inférieure");
		printf ("\n DIP 1 : Corde supérieure"); 
		printf ("\n DIP 2 : ---");
		printf ("\n DIP 3 : Écouter la note de la corde");
		printf ("\n \n NOTE : Le numéro de la corde sélectionnée est affiché en binaire sur les LEDs. \n");

}

/********************************************************************************************
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Permet d'afficher en binaire le numéro de la corde sélectionnée sur les LED 
	ENTRÉE : 
	 noCorde : Numéro de la corde de guitare sélectionnée
********************************************************************************************/
void ajusterLED(int noCorde) 
{
	int ledNo, base;

	// Ajustement du numéro de la corde sur les trois premières LED
	for (ledNo = 2; ledNo>=0; ledNo--) { // Pour chacun des 3 LEDS
		base = (int) pow(2,ledNo);		// Algèbre binaire pour trouver les LEDs à allumer
		if ( base <= noCorde)			
		{
			noCorde = noCorde-base; 
			DSK6713_LED_on(ledNo);
		}
		else DSK6713_LED_off(ledNo); 	// Eteint la LED associé au bit
	}

	return;
}

/********************************************************************************************
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Permet de faire l'acquisition des commandes utilisateurs.
     Le code contient un algorithme d'anti-rebond.
********************************************************************************************/
int getCommandes() 
{
	int isNew;		// Sortie de la fonction (flag : 1 si une nouvelle commande)
	int noDIP;		// Numéro de la DIP appuyé

	isNew = getNewCommand(&noDIP); // flag : 1 si nouvelle commande (algo anti-rebond)

	// Segment de code pour le choix de la corde 
	if(isNew)	// si une nouvelle commande

		switch (noDIP) {
			case 0 : // si la commande est la DIP #0
				Commandes.noCorde = max(Commandes.noCorde-1,1); // La nouvelle corde est la corde inférieure
				isNew = 1;
			break;
			case 1 : // si la commande est la DIP #1
				Commandes.noCorde = min(Commandes.noCorde+1,NB_CORDES); // La nouvelle corde est la corde supérieure
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

	// Segment de code pour l'écoute d'une note
	if (DSK6713_DIP_get(3) == 0)  // S'il y a une écoute
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
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:   
    DESCRIPTION : 
	 Permet d'écouter la fréquence fondamentale de la note de la corde.
	ENTRÉE : 
	 noCorde : Numéro de la corde de guitare sélectionnée
	 isPlay : Flag (1 si une fréquence fondamentale est présentement générée)
    RETOUR : 
	 Échantillon de sortie
	NOTE : 
	 Méthode avec la fonction cos() de la librairie standard <math.h>
********************************************************************************************/
float  genFrqCos(int noCorde, int isPlay) 
{
	float echAmp; 						// Amplitude de l'échantillon
	static int memIsPlay = 0;			// Mémoire de isPlay
	static float angle;					// Angle

	if (isPlay == 1 && memIsPlay == 0) 	 // Si c'est le début d'une écoute de note
		angle = 0;  					 // Initialisation de l'angle
	else	{}

	echAmp = genCos(DeltaAngle[noCorde-1], &angle);
	memIsPlay = isPlay;

	return echAmp;
}

/********************************************************************************************
  Créateur:    
  Date:        
  Revisions: 
 DESCRIPTION : Permet d'écouter la fréquence fondamentale de la note de la corde 
 NOTE : Vous devez choisir une méthode et l'implémenter
********************************************************************************************/
float  genFrq_methode2(int noCorde, int isPlay) 
{
	// ÉCRIVEZ VOTRE CODE ICI 
	// NOTE : Vous devez créer un .c contenant à la fois les fonctions
	// genM2(?) et initGenM2(?) (fct. de génération et fct. d'initialisation)
	// Vous devez aussi créer un fichier .dat
	// Inspérez-vous de la fonction genFrq_cos(...)
	/*==========================================================================*/

    float echAmp;                       // Amplitude de l'échantillon
    static int memIsPlay = 0;           // Mémoire de isPlay
    static struct complx vecteur_tourn;            // vecteur tournant
    static int incr = 0;

    if (isPlay == 1 && memIsPlay == 0){    // Si c'est le début d'une écoute de note
        vecteur_tourn.real = 1;
        vecteur_tourn.img = 0;
        incr = 0;
    }

    if(incr >= nb[noCorde-1]){
        vecteur_tourn.real = 1;
        vecteur_tourn.img = 0;
        incr = 0;
    }

    echAmp = genCosM2(C_delta[noCorde-1],&vecteur_tourn);
    memIsPlay = isPlay;
    incr++;
    return echAmp;

}
