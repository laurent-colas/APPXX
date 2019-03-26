/* IIR2_2ndOrder_directII.c */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        16 juillet 2009
  Revisions:

  DESCRIPTION :
     Réalisation d'un filtre IIR biquad de forme direct type II

  ENTRÉE :
     x : Amplitude de l'échantillon audio à filtrer
     w : Variables intermédiaires w[n], w[n-1] et w[n-2]
     C : Coefficients du filtre b0, b1, b2, a0, a1 et a2

  RETOUR :
     Amplitude de l'échantillon audio filtré
*/

#include <stdio.h>
#include <math.h>
#include "IIR_2ndOrder_directII.h"

int sat_25bits(int x);

int IIR_2ndOrder_directII(int x, int w[], const short C[])
{

    /* TRÈS IMPORTANT :
              Ce code a été écrit pour simplifier la conversion manuelle
              en assembleur. Pour être optimal lors de la conversion,
              essayez d'abord de reproduire ce code en assembleur. Une
              fois terminé, essayez ensuite de l'optimiser. Si vous
              partez de zéro, vous risquez de prendre beaucoup de temps
              à corriger des problèmes de retournement (overflow).

              Notez bien les formats des variables (long vs. int
              etc.) et choisissez bien vos instructions assembleurs (
              ex : MPY vs. MPYI et LDH vs. LDW).

              Finalement, il est essentiel de bien comprendre le code,
              et ce, particulièrement au niveau des changements de
              format fractionnaire. */

    int y;
    int memA, memB, memC;   // Mémoire pour entier de 32 bits
    long lmem;              // Mémoire pour entier de 40 bits (max. du DSP en point-fixe)
    /*******************************************************************************
    w(n) = a0*x(n)-a1*w(n-1)-a2*w(n-2)
    *******************************************************************************/

    /* Saturation de l'entrée à 25 bits pour ne pas dépasser le format 11Q13
      (nécessaire pour que les opérations qui suivent ne dépassent pas 40 bits) */
    x = sat_25bits(x);

    // a0*x(n)
    lmem = (long)C[3]*(long)x;          // Format 2Q13 x 11Q13 = 13Q26  (format fractionnaire)
        // NOTEZ QUE LE FORMAT 13Q26 EST À L'INTÉRIEUR DES 40 BITS DE LA VARIABLE lmem

    memA = (int)(lmem>>13);             // Format 13Q13  (équivalent à un décalage de 13 bits)


    // a1*w(n-1)
    lmem = (long)C[4]*(long)w[1];       // Format 2Q13 x 11Q13 = 13Q26


    memB = (int)(lmem>>13);             // Format 13Q13




    // a0*x(n)-a1*w(n-1)
    memC = memA-memB;                   /* Format 14Q13 (Peut prendre ce format si les deux
                                           variables sont de même signe et ont des amplitudes
                                           très élevée)*/




    // a2*w(n-2)
    lmem = (long)C[5]*(long)w[2];       // Format 2Q13 x 11Q13 = 13Q26



    memB = (int)(lmem>>13);             // Format 13Q13


    // a0*x(n)-a1*w(n-1)-a2*w(n-2)
    memA = memC-memB;                   // Format 15Q13


    memB = sat_25bits(memA); /* NOTE : Si la saturation ce produit ici, il est possible
                                       que le filtrage ne fonctionne pas. Pour régler
                                       le problème utilisez des coefficients de filtre
                                       qui évitent les dépassements (voir aide de matlab
                                       pour la fonction ss2sos()) */


    w[0] = memB;                       // Enregistremet de la variable intermédiaire

    /*******************************************************************************
    y(n) = b0*w(n)+b1*w(n-1)+b2*w(n-2)
    *******************************************************************************/

    // b0*w(n)
    lmem = (long)C[0]*(long)w[0];       // Format 2Q13 x 11Q13 = 13Q26


    memA = (int)(lmem>>13);             // Format 13Q13


    // b1*w(n-1)
    lmem = (long)C[1]*(long)w[1];       // Format 2Q13 x 11Q13 = 13Q26



    memB = (int)(lmem>>13);             // Format 13Q13


    // b0*w(n)+b1*w(n-1)
    memC = memA+memB;                   // Format 14Q13


    // b2*w(n-2)
    lmem = (long)C[2]*(long)w[2];       // Format 2Q13 x 11Q13 = 13Q26


    memA = (int)(lmem>>13);             // Format 13Q13


    // y(n)= b0*w(n)+b1*w(n-1)+b2*w(n-2)
    y = memC+memA;                      // Format 15Q13
    /* NOTE : Comme le gain des filtres dans la bande passante est unitaire, la valeur
       de y sera probablement dans le format 11Q13, car x entre dans le format 11Q13.*/

    /*******************************************************************************
    Décalage des variables intermédiaires w
    *******************************************************************************/
    w[2] = w[1];
    w[1] = w[0];

    return y;
}

/***********************************************************************************
DESCRIPTION : Permet de faire une saturation à 25bits
***********************************************************************************/
int sat_25bits(int x) {

    int r;

    if (x>(pow(2,24)-1))
        r =(int)pow(2,24)-1;
    else if (x<-pow(2,24))
        r = (int)-pow(2,24);
    else
        r = (int)x;

    return r;
}



