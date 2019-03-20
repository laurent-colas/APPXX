/* genCos.h */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        7 juin 2009
  Revisions:   

  DESCRIPTION : 
     Permet de générer un signal sinusoïdal en utilisant la 
     fonction cosinus de la librairie standard <math.h>
*/


#ifndef GENCOS_H_
#define GENCOS_H_

/*************************************************************
 Liste des prototypes de fonction
*************************************************************/

// DESCRIPTION : Génération de l'amplitude de l'éch. à l'aide de la fonction cos de 
// la librairie standard <math.h>
float genCos(float deltaAngle, float *anglePtr);

// DESCRIPTION : Permet de garder l'angle entre 0 et 2pi
void fixAngle(float *a_ptr);

#endif /* GENCOS_H_ */
