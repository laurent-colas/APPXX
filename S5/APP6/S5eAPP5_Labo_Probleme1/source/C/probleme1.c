/********************************************************************************************
Version 1.0
Auteur : Bruno Gagnon
Date de cr�ation : 16 juillet 2009
Date de la derni�re modification : 11 octobre 2010
Modif� par : Louis-Charles Caron
Description : Programme de d�part pour la question 1 du labo de l'APP accordeur
Propri�t� : Universit� de Sherbrooke, d�partement de g�nie �lectrique et informatique
********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dsk6713_AIC23.h"  // pour l'utilisation AIC
#include "probleme1.h"
#include "bitrev_index.h"
#include "C6713Helper_UdeS.h"

// Variables globales pour utiliser AIC du DSK
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ; 	// Fr�quence d'�chantillonnage selon les d�finitions du DSK
#define DSK6713_AIC23_INPUT_LINE 0x0011
#define DSK6713_AIC23_INPUT_MIC 0x0015
Uint16 inputsource=DSK6713_AIC23_INPUT_MIC; // selection de l'entr�e

// Constantes globales
#define PI 3.141592653589793
#define Fs 8000

// Param�tres et variables pour la premi�re sinuso�de
#define F1 1500
#define L1 16
int TableCos1[L1];

/********************************************************************************************
Description : Fonction principale
********************************************************************************************/
int main()
{
	int n;

	// Amplitudes pour la g�n�ration du premier signal sinuso�dal
	for (n=0; n<L1; n++)
		TableCos1[n] = (int)1000*cos(2*PI*F1*n/Fs);

	// Amplitudes pour la g�n�ration du deuxi�me signal sinuso�dal

	// Calcul de la FFT du deuxi�me signal sinuso�dal
	calculFFT();

	// D�marrage des interruptions
	comm_intr();
	while(1);

}

/********************************************************************************************
Description : Code execut� � toutes les interruptions
********************************************************************************************/
interrupt void c_int11()
{
	int x;

	x = getEchantillon(TableCos1, L1);

	output_left_sample(x);
	
	return;

}

/********************************************************************************************
Description : Code pour obtenir l'�chantillon courrant
********************************************************************************************/
// Vous devez �crire cette fonction
int getEchantillon(int tableau[], int N)
{
	int out;
	static int compteur = 0;
   
	out = tableau[compteur];

	// Ajouter le code pour incr�menter le compteur d'�chantillons
	// ...
	// ...

	return out;
}

/********************************************************************************************
Description : Code pour calculer la FFT du second signal sinuso�dal
********************************************************************************************/
// Vous devez �crire cette fonction
void calculFFT()
{
	// Remplissez les cases paires du tableau de floats avec les valeurs de la table de cosinus � 250Hz et les cases impaires avec des 0.

	// Calculez la FFT sur votre tableau de floats en utilisant la fonction de FFT choisie

	// G�n�rez les valeurs d�index en utilisant la fonction bitrev_index()

	// R�ordonnez les valeurs de sortie de la FFT en utilisant la fonction DSPF_sp_bitrev_cplx()

}
