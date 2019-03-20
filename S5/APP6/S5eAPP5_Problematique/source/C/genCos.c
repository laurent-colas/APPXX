/* genCos.c */
/*
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        7 juin 2009
  Revisions:   

  DESCRIPTION : 
     Permet de g�n�rer un signal sinuso�dal en utilisant la 
     fonction cosinus de la librairie standard <math.h>

  ENTR�E : 
     deltaAngle : valeur de 2*PI*f/Fs

  ENTR�E/SORTIE :
	 angle_Ptr : Pointeur sur l'angle courant de la fonction cos. 
	             Normalement, ce pointeur doit pointer sur une variable 
	             statique ou globale.

  RETOUR : 
	 Amplitude de l'�chantillon courrant pour la g�n�ration du signal.
	 
*/

#include <stdio.h>
#include <math.h>
#include "CONSTANTES.h" // Constantes partag�es
#include "genCos.h"


const float PI = 3.14159265358979;

/***********************************************************
 Permet de garder l'angle entre 0 et 2pi
***********************************************************/
void fixAngle(float *a_Ptr) {

	if (*a_Ptr >= 2*PI)
		*a_Ptr = *a_Ptr-2*PI;
	else {}

	return;
}


float genCos(float deltaAngle, float *angle_Ptr) {

	float out; // �chantillon de sortie

	*angle_Ptr = ( *angle_Ptr + deltaAngle ); // calcul de l'angle de l'�ch. actuel
	fixAngle(angle_Ptr);		// permet de garder l'angle entre 0 et 2pi
	out = cos(*angle_Ptr);		// amplitude de sortie

	return out;
}


