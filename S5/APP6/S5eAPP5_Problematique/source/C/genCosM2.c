/*
 * genM2.c
 *
 *  Created on: Mar 24, 2019
 *      Author: berth
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "CONSTANTES.h" // Constantes partagées
#include "genCosM2.h"





#define nb_max 98
extern float PI;
extern int fe;
extern float F0_NOMINAL;
extern int incrGlb;

//struct complx;

struct complx C_delta[NB_CORDES];
int nb[NB_CORDES] = {98,73,55,41,33,25};
FILE * file;


void initGenM2(){
    //créaction unique des pas pour chaque corde
    C_delta[0].real = cos(2*PI*82.407/fe);
    C_delta[1].real = cos(2*PI*110.000/fe);
    C_delta[2].real = cos(2*PI*146.832/fe);
    C_delta[3].real = cos(2*PI*195.998/fe);
    C_delta[4].real = cos(2*PI*246.942/fe);
    C_delta[5].real = cos(2*PI*329.628/fe);

    C_delta[0].img = sin(2*PI*82.407/fe);
    C_delta[1].img = sin(2*PI*110.000/fe);
    C_delta[2].img = sin(2*PI*146.832/fe);
    C_delta[3].img = sin(2*PI*195.998/fe);
    C_delta[4].img = sin(2*PI*246.942/fe);
    C_delta[5].img = sin(2*PI*329.628/fe);

    // créaction du signal et écriture dans un fichier "CosM2.dat"
    int m = 0;
    int i = 0;

    file = fopen("CosM2.dat","w2");
    for(m = 0; m <= NB_CORDES-1 ; m++){
        struct complx VT[nb_max];
        VT[0].real = 1;
        VT[0].img = 0;

        fprintf(file, "%s, %d, %s", "Corde: " , m+1 ,"\n");
        fprintf(file, "%f, %s",VT[0].real,"\n");
        for(i = 1; i <= nb[m]-1 ; i++){
            VT[i].real = VT[i-1].real*C_delta[m].real - VT[i-1].img*C_delta[m].img;
            VT[i].img = VT[i-1].img*C_delta[m].real + VT[i-1].real*C_delta[m].img;
            fprintf(file, "%f, %s",VT[i].real,"\n");
        }
        fprintf(file, "%s, %s", "====================" ,"\n");
    }
    fclose(file);
}

float genCosM2(struct complx C_delta, struct complx *VT){
    float out;

     // Ajout du pas pour l'incrémentaiton du vecteur tournant
     (*VT).real = (*VT).real * C_delta.real - (*VT).img * C_delta.img;
     (*VT).img = (*VT).img * C_delta.real + (*VT).real * C_delta.img;
     out = ((*VT).real)/(sqrt(((*VT).real*(*VT).real)+((*VT).img*(*VT).img)));

    return out;
}


