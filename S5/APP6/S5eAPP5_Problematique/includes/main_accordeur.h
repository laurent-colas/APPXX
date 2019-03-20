/* main_accordeur.h */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:  

  DESCRIPTION : 
     Fichier d'entêtes principal pour un accordeur de guitare.
     Permet d'écouter les notes de guitare et d'obtenir l'erreur 
     sur l'accordement.
*/

#ifndef MAIN_ACCORDEUR_H_
#define MAIN_ACCORDEUR_H_

/*************************************************************
 Liste des structures
*************************************************************/

// Structure pour contenir les commandes de l'utilisateur
typedef struct  
{
 	int noCorde;		/* Numéro de la corde à accorder 
							corde #1 @ f=82.407 Hz
							corde #2 @ f=110.000 Hz
							corde #3 @ f=146.832 Hz
							corde #4 @ f=195.998 Hz
							corde #5 @ f=246.942 Hz
							corde #6 @ f=329.628 Hz */
 	int isPlay;			// 1 si l'utilisateur désire écouter la note
} USELECTIONS;

/*************************************************************
 Liste des prototypes de fonction
*************************************************************/
void attendre(float seconds);
interrupt void c_int11();
void initAccordeur();
void afficherMenu();
void ajusterLED(int noCorde);
int getCommandes();
float genFrqCos(int , int);
float genFrq_methode2(int , int);
int min(int a, int b);
int max(int a, int b);

#endif /* MAIN_ACCORDEUR_H_ */


