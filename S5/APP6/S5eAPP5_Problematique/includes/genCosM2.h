/*
 * genCosM2.h
 *
 *  Created on: Mar 24, 2019
 *      Author: berth
 */

#ifndef INCLUDES_GENCOSM2_H_
#define INCLUDES_GENCOSM2_H_


/*************************************************************
 Liste des prototypes de fonction
*************************************************************/

// DESCRIPTION : Génération de l'amplitude de l'éch. à l'aide de la fonction cos de
// la librairie standard <math.h>
float genCosM2(float deltaAngle, float *anglePtr);

// DESCRIPTION : Permet de garder l'angle entre 0 et 2pi
void fixAngleM2(float *a_ptr);

// DESCRIPTION : Permet de garder l'angle entre 0 et 2pi
void initGenM2(float f, float fe);


#endif /* INCLUDES_GENCOSM2_H_ */
