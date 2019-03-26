/* main.c */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        16 juillet 2009
  Revisions:

  DESCRIPTION :
     Appeler des filtres IIR d'ordre 2 pour faire une cascade de filtres
     et appliquer le gain global.

  ENTRÉE :
     noCorde : Numéro de la corde de guitare sélectionnée
     x : Amplitude de l'échantillon audio à filtrer
     coeffsIIR.dat : fichier contenant l'information des filtres

  RETOUR :
     Amplitude de l'échantillon audio filtré
*/

#include <stdio.h>
#include <math.h>
#include "IIR_2ndOrder_directII.h"
#include "filtrerCascadeIIR.h"
#include "coeffsIIR.dat"

/***********************************************************************************
DESCRIPTION : Appeler des filtres IIR d'ordre 2 pour faire une cascade de filtres
              et appliquer le gain global
***********************************************************************************/
short filtrerCascadeIIR(int noCorde, short x)
{
    int n, ligne;
    int y;

    y = x;

    // Pour chacune des sections d'ordre 2 du filtre
    for (n=0; n<IIR_NB_ORDRE2[noCorde-1]; n++) {
        // filtrage par le IIR biquad de structure direct II
        ligne = IIR_NO_LIGNE[noCorde-1]+n;
        // en parametre on lui donne le signal recus y, les variables intermediaire et les coefficients du filtres
        y = IIR_2ndOrder_directII_ASM(y, &IIR_W[n][0], &IIR_COEFFS[ligne][0]); // y->15Q13
//        y = IIR_2ndOrder_directII(y, &IIR_W[n][0], &IIR_COEFFS[ligne][0]); // À CODER
    }

    // Appliquer le gain global
    y = (int)IIR_GAINS[noCorde-1]*y; // 2Q13 x 15Q13 = 17Q26
    y = (y>>13);                     // y->17Q13

    return sat_16bits(y);            // y->2Q13
}

/***********************************************************************************
DESCRIPTION : Initialisation des variables intermédiaires w(n-i)
***********************************************************************************/
void init_w()
{
    int i, j;

    for (i=0; i<IIR_NB_SECTIONS_MAX; i++)   {
        for (j=0; j<3; j++)     IIR_W[i][j] = 0;
    }
}

/***********************************************************************************
DESCRIPTION : Permet de faire une saturation à 16bits
***********************************************************************************/
short sat_16bits(int x) {

    short r;

    if (x>(pow(2,15)-1))
        r =(short)pow(2,15)-1;
    else if (x<-pow(2,15))
        r = (short)-pow(2,15);
    else
        r = (short)x;

    return r;
}


