/* findErrAccordage.c */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        12 juin 2009
  Revisions:   

  DESCRIPTION : 
	 Code donnant l'erreur en cents sur la note jouée par rapport à 
	 la note nominale
*/

#ifndef FINDERRACCORDAGE_H_
#define FINDERRACCORDAGE_H_

/*************************************************************
 Liste des prototypes de fonction
*************************************************************/

// Description : Fonction principale pour trouver l'erreur d'accordement
float findErrAccordage(float tampon[], float frqDesiree);

// Description : Effectue l'autocorrélation du tampon circulaire d'échantillons.
void faireAutocorr(float bloc[], float resultat[]);

// Description : Recherche la fréquence fondamentale à partir du résultat de l'autocorrélation
float getF0(float xx[], float fDesirer);

// À vous de coder cette fonction!!!!!!!
void faireAutocorr_fft(float bloc[], float resultat[]);

#endif /* FINDERRACCORDAGE_H_*/
