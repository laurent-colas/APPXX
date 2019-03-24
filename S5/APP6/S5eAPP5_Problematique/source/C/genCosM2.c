/*
 * genM2.c
 *
 *  Created on: Mar 24, 2019
 *      Author: berth
 */

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "CONSTANTES.h" // Constantes partagées
#include "genCosM2.h"


const float PI = 3.14159265358979;
float complex C_delta = 0;
int nb = 0;
FILE * file;


void initGenM2(float f, float fe){
    C_delta = (float) exp((double) ((2*I*PI)*(f/fe)));
    nb = floor(fe/f);
    float VT[nb];
    VT[0] = 1;

    int i = 0;
    for(i = 1; i <= nb ; i++){
        VT[i] = crealf(VT[i-i]*C_delta);
    }

    file = fopen("CosM2.dat","w2");
    fprintf(file, "%f", VT);
    fclose(file);
}
