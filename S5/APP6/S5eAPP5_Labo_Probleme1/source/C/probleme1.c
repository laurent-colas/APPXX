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
#include "DSPF_sp_cfftr2_dit.h"
#include "DSPF_sp_bitrev_cplx.h"
#include "twiddles.h"


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
#define F2 250
#define L2 32

int TableCos1[L1];
int TableCos2[L2];
float TableCos3[L2*2];
float Xm[L2];
short index[8];
int compteur = 0;

#pragma DATA_ALIGN(TableCos3,8);

/********************************************************************************************
Description : Fonction principale
********************************************************************************************/
int main()
{
	int n;

	// Amplitudes pour la g�n�ration du premier signal sinuso�dal
	for (n=0; n<L1; n++)
		TableCos1[n] = (int)1000*cos(2*PI*F1*n/Fs);
	for (n=0; n<L2; n++)
        TableCos2[n] = (int)1000*cos(2*PI*F2*n/Fs);

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
	short y;

	//x = (short) getEchantillon(TableCos1, L1);
	y = (short) getEchantillon(TableCos2, L2);

	/*output = x;
	output = output<<16 | y;
	output_sample(output);*/

	output_left_sample(y);
	
	return;

}

/********************************************************************************************
Description : Code pour obtenir l'�chantillon courrant
********************************************************************************************/
// Vous devez �crire cette fonction
int getEchantillon(int tableau[], int N)
{
	int out;
	//static int compteur = 0;
   
	out = tableau[compteur];

	// Ajouter le code pour incr�menter le compteur d'�chantillons
	// ...
	// ...
	compteur = compteur + 1;
    if (compteur >= N)
    {
        compteur = 0;
    }

	return out;
}

/********************************************************************************************
Description : Code pour calculer la FFT du second signal sinuso�dal
********************************************************************************************/
// Vous devez �crire cette fonction
void calculFFT()
{
    int n;
	// Remplissez les cases paires du tableau de floats avec les valeurs de la table de cosinus � 250Hz et les cases impaires avec des 0.
    for (n = 0; n < L2;n++)
    {
        TableCos3[2*n] = TableCos2[n];
        TableCos3[2*n+1] = 0.;
    }


	// Calculez la FFT sur votre tableau de floats en utilisant la fonction de FFT choisie

    DSPF_sp_cfftr2_dit(TableCos3, w, L2);


	// G�n�rez les valeurs d�index en utilisant la fonction bitrev_index()

    bitrev_index(index, L2);

	// R�ordonnez les valeurs de sortie de la FFT en utilisant la fonction DSPF_sp_bitrev_cplx()

    DSPF_sp_bitrev_cplx((double *)TableCos3, index, L2);

    for (n = 0; n < L2; n++)
    {
        Xm[n] = sqrt(TableCos3[2*n]*TableCos3[2*n] + TableCos3[2*n+1]*TableCos3[2*n+1]);
    }
    n = 0;

}
