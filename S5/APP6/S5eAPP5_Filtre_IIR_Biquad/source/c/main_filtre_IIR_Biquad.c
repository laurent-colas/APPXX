/* main.c */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        16 juillet 2009
  Revisions:   Paul Charette (automne 2016)
   	   	   	   - ajouté du code dans direct1FIR_ASM.asm qui fait
   	   	   	     la protection du contexte et la programmation de l’AMR
   	   	   	     car les étudiants ont déjà vu ces notions dans l’APP2 et ainsi
   	   	   	     accélérer la résolution du Labo qui est toujours trop long.
   	   	   	   - ajouté des flags dans c_int11() pour prendre un input interne/externe
   	   	   	     et inclure/exclure les fonctions de filtrage, afin de pouvoir
   	   	   	     facilement comparer les résultats obtenus avec le code donné
   	   	   	     versus avec le code développé.

  DESCRIPTION : 
     Permet de filtrer un signal sinusoïdal qui est généré à l'interne 
     via le bouton glissoir GEL du DSK

  ENTRÉE : 
     valeur du bouton glissoir GEL
	 Pour accéder au bouton : 
	    1) "File/Load GEL..."   et choisir frequence.gel dans ..\Filtre_IIR_Biquad
		2) "GEL/Sine_Frequency" et choisir "Frq"w

  SORTIE :
	 - Amplitude du signal à l'entrée du filtre sur canal droit de la sortie 
	   HEADPHONE du DSK
	 - Amplitude du signal de sortie du filtre sur canal gauche de la sortie 
	   HEADPHONE du DSK
*/	 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dsk6713_aic23.h"
#include "C6713Helper_UdeS.h"
#include "filtrerCascadeIIR.h"
#include "direct1FIR.h"
#include "direct1FIR_ASM.h"
#include "direct1FIRcoeffs.dat"

/*********************************************
Variables globales pour utiliser AIC du DSK
*********************************************/
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ; 	// Fréquence d'échantillonnage selon les définitions du DSK
#define DSK6713_AIC23_INPUT_LINE 0x0011
#define DSK6713_AIC23_INPUT_MIC 0x0015
Uint16 inputsource=DSK6713_AIC23_INPUT_MIC; // selection de l'entrée

#define GAUCHE 0 // Haut-parleur gauche
#define DROIT  1 // Haut-parleur droit
union {Uint32 uint; short channel[2];} AIC23_data; // Pour contenir les deux signaux

/*********************************************
// Variables globales
*********************************************/
const double PI =3.141592653589793;
const int Fs = 8000;
int noCorde=1; // Numéro de la corde de guitare, permet de sélectionner le filtre désiré
/* NOTE : Vous pouvez changer la valeur de noCorde. Pour ce faire, utilisez "View/Watch Window"
          et ajoutez la variable noCorde dans "Watch 1" */

#define TAMPON_L  64
#pragma DATA_ALIGN(tampon, TAMPON_L*2); // Requis pour l'adressage circulaire en assembleur
short tampon[TAMPON_L]={0}; 		// Tampon d'échantillons
short *pTampon=&tampon[TAMPON_L-1];	// Pointeur sur l'échantillon courant


// Fréquence générée avec le GEL slider
float frq=0;

/********************************************************************************************
Description : Fonction principale
********************************************************************************************/
int main()
{
	// Initialisation des variables intermédiaires w(n-i)
	init_w();

	// Démarrage des interruptions
	comm_intr();
	while(1);
}

/********************************************************************************************
Description : Code executé à toutes les interruptions
********************************************************************************************/
interrupt void c_int11()
{
	static int inputInterne = 1, FIR_ASM = 0, FLT_IIR = 0;
	short x, xFIR, y;
	static int n=1;

	// Échantillon d'entrée interne/externe
	if (inputInterne)
		// Génération de la fréquence désirée provenant du bouton glissoir GEL
		x = (short)(2000*cos(2*PI*frq/Fs*n++));
	else
		// Capture de l'échantillon provenant de l'entrée LINE IN
		x = (short) input_sample();

	// Filtrage FIR passe-haut en assembleur ou en C
	if (FIR_ASM)
		// Dans le code fourni, cette fonction fait simplement incrémenter le
		// pointeur pTampon et retourner l'échantillon d'entrée, x,
		// dans la valeur de sortie, xFIR - i.e. c'est un simple "pass-through"
		// qui ne fait pas de filtrage.
		pTampon = direct1FIR_ASM(pTampon, x, CoeffsFIR, &xFIR);
	else
		pTampon = direct1FIR(pTampon, x, CoeffsFIR, FIRNbCOEFFS, tampon, TAMPON_L, &xFIR);

	// Filtrage IIR biquad, ou pas
	if (FLT_IIR)
		y = filtrerCascadeIIR(noCorde, xFIR);
	else
		y = xFIR;

	AIC23_data.channel[DROIT] =  x;		   	// Échantillon d'entrée sur le canal droit
	AIC23_data.channel[GAUCHE] = y;		   	// Échantillon filtré sur canal gauche

	output_sample(AIC23_data.uint);			// Sortir les deux signaux sur HEADPHONE

	return;
}
