/* findErrAccordage.c */
/*
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        12 juin 2009
  Revisions:   

  DESCRIPTION : 
	 Code donnant l'erreur en cents sur la note jou�e par rapport � 
	 la note nominale
*/

#ifndef FINDERRACCORDAGE_H_
#define FINDERRACCORDAGE_H_

/*************************************************************
 Liste des prototypes de fonction
*************************************************************/

// Description : Fonction principale pour trouver l'erreur d'accordement
float findErrAccordage(float tampon[], float frqDesiree);

// Description : Effectue l'autocorr�lation du tampon circulaire d'�chantillons.
void faireAutocorr(float bloc[], float resultat[]);

// Description : Recherche la fr�quence fondamentale � partir du r�sultat de l'autocorr�lation
float getF0(float xx[], float fDesirer);

// � vous de coder cette fonction!!!!!!!
void faireAutocorr_fft(float bloc[], float resultat[]);

#endif /* FINDERRACCORDAGE_H_*/
