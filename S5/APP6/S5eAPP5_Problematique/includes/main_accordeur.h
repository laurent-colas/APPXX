/* main_accordeur.h */
/*
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        2 septembre 2009
  Revisions:  

  DESCRIPTION : 
     Fichier d'ent�tes principal pour un accordeur de guitare.
     Permet d'�couter les notes de guitare et d'obtenir l'erreur 
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
 	int noCorde;		/* Num�ro de la corde � accorder 
							corde #1 @ f=82.407 Hz
							corde #2 @ f=110.000 Hz
							corde #3 @ f=146.832 Hz
							corde #4 @ f=195.998 Hz
							corde #5 @ f=246.942 Hz
							corde #6 @ f=329.628 Hz */
 	int isPlay;			// 1 si l'utilisateur d�sire �couter la note
} USELECTIONS;



const short coef [255] = {4,
                          4,
                          5,
                          5,
                          5,
                          6,
                          6,
                          6,
                          7,
                          7,
                          8,
                          8,
                          9,
                          9,
                         10,
                         10,
                         11,
                         11,
                         12,
                         13,
                         13,
                         14,
                         14,
                         15,
                         16,
                         16,
                         17,
                         18,
                         18,
                         19,
                         19,
                         20,
                         20,
                         21,
                         21,
                         22,
                         22,
                         22,
                         22,
                         22,
                         22,
                         22,
                         21,
                         21,
                         20,
                         19,
                         18,
                         17,
                         16,
                         14,
                         13,
                         11,
                          9,
                          7,
                          4,
                          2,
                         -1,
                         -4,
                         -8,
                        -11,
                        -15,
                        -19,
                        -23,
                        -28,
                        -33,
                        -38,
                        -43,
                        -49,
                        -54,
                        -60,
                        -67,
                        -73,
                        -80,
                        -87,
                        -94,
                       -101,
                       -109,
                       -117,
                       -125,
                       -133,
                       -141,
                       -149,
                       -158,
                       -167,
                       -176,
                       -185,
                       -194,
                       -203,
                       -212,
                       -222,
                       -231,
                       -240,
                       -250,
                       -259,
                       -268,
                       -278,
                       -287,
                       -296,
                       -305,
                       -314,
                       -323,
                       -332,
                       -340,
                       -348,
                       -357,
                       -365,
                       -372,
                       -380,
                       -387,
                       -394,
                       -401,
                       -407,
                       -413,
                       -419,
                       -424,
                       -429,
                       -434,
                       -438,
                       -442,
                       -445,
                       -448,
                       -451,
                       -454,
                       -455,
                       -457,
                       -458,
                       -459,
                      32313,
                       -459,
                       -458,
                       -457,
                       -455,
                       -454,
                       -451,
                       -448,
                       -445,
                       -442,
                       -438,
                       -434,
                       -429,
                       -424,
                       -419,
                       -413,
                       -407,
                       -401,
                       -394,
                       -387,
                       -380,
                       -372,
                       -365,
                       -357,
                       -348,
                       -340,
                       -332,
                       -323,
                       -314,
                       -305,
                       -296,
                       -287,
                       -278,
                       -268,
                       -259,
                       -250,
                       -240,
                       -231,
                       -222,
                       -212,
                       -203,
                       -194,
                       -185,
                       -176,
                       -167,
                       -158,
                       -149,
                       -141,
                       -133,
                       -125,
                       -117,
                       -109,
                       -101,
                        -94,
                        -87,
                        -80,
                        -73,
                        -67,
                        -60,
                        -54,
                        -49,
                        -43,
                        -38,
                        -33,
                        -28,
                        -23,
                        -19,
                        -15,
                        -11,
                         -8,
                         -4,
                         -1,
                          2,
                          4,
                          7,
                          9,
                         11,
                         13,
                         14,
                         16,
                         17,
                         18,
                         19,
                         20,
                         21,
                         21,
                         22,
                         22,
                         22,
                         22,
                         22,
                         22,
                         22,
                         21,
                         21,
                         20,
                         20,
                         19,
                         19,
                         18,
                         18,
                         17,
                         16,
                         16,
                         15,
                         14,
                         14,
                         13,
                         13,
                         12,
                         11,
                         11,
                         10,
                         10,
                          9,
                          9,
                          8,
                          8,
                          7,
                          7,
                          6,
                          6,
                          6,
                          5,
                          5,
                          5,
                          4,
                          4};

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
short * FIR_ASM (short *cPtr, short ech, const short h[], short *y);

#endif /* MAIN_ACCORDEUR_H_ */


