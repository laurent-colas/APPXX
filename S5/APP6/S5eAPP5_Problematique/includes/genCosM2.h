/*
 * genCosM2.h
 *
 *  Created on: Mar 24, 2019
 *      Author: berth
 */

#ifndef INCLUDES_GENCOSM2_H_
#define INCLUDES_GENCOSM2_H_


struct complx{
    float real;
    float img;
};

/*************************************************************
 Liste des prototypes de fonction
*************************************************************/

// DESCRIPTION : Génération de l'amplitude de l'éch. à l'aide de la fonction cos de
// la librairie standard <math.h>
float genCosM2(struct complx C_delta, struct complx *VT);

// DESCRIPTION : Permet de garder l'angle entre 0 et 2pi
//void fixAngleM2(float *a_ptr);

// DESCRIPTION : Permet de garder l'angle entre 0 et 2pi
void initGenM2();


#endif /* INCLUDES_GENCOSM2_H_ */
