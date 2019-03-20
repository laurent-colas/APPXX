/* main.c */
/*
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        16 juillet 2009
  Revisions:   

  DESCRIPTION : 
     Appeler des filtres IIR d'ordre 2 pour faire une cascade de filtres
     et appliquer le gain global.

  RETOUR :
     Amplitude de l'�chantillon audio filtr�
*/

#ifndef FILTRERCASCADEIIR_H_
#define FILTRERCASCADEIIR_H_

/*************************************************************
Liste des prototypes de fonction
*************************************************************/

/* Description : Appeler des filtres IIR d'ordre 2 pour faire une cascade 
                 de filtres et appliquer le gain global */
short filtrerCascadeIIR(int noCorde, short x);

// Description : Initialisation des variables interm�diaires w(n-i)
void init_w();

// Description : Permet de faire une saturation � 16bits
short sat_16bits(int x);

#endif /* FILTRERCASCADEIIR_H */
