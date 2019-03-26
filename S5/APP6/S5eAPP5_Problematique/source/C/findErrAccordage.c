/* findErrAccordage.c */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        12 juin 2009
  Revisions:   

  DESCRIPTION : 
	 Code donnant l'erreur en cents sur la note jouée par rapport à 
	 la note nominale.

  ENTRÉES : 
	bloc[] : Tampon contenant les éhantillons audio utilisés pour 
	         trouver l'erreur d'accordement

	frqDesirer : Fréquence fondamentale nominale désirée pour la corde

  RETOUR :
	Erreur d'accordage de la corde en cents

  NOTE : La cents représente une distance entre deux fréquences sur une échelle 
         logarithmique. L'équation est :
         
              DistCents = 1200*log2(f1/f2)
*/

#include <stdio.h>
#include <math.h>
#include "findErrAccordage.h"
#include "fenetreHanning.h" // Fenêtre de Hanning pré-calculée
#include "CONSTANTES.h"		  // Constantes partagées
// Includes pour la FFT
#include "twiddle_512.h"
#include "DSPF_sp_cfftr2_dit.h"
#include "DSPF_sp_bitrev_cplx.h"
#include "DSPF_sp_icfftr2_dif.h"


float findErrAccordage(float bloc[], float frqDesiree) {

	/*   NOTE : Le calcul de l'erreur d'accordage prend plus de temps 
	            que la durée d'une période d'échantillonnage. */

	int i;
	float f0;							// Fréquence fondamentale de la note jouée
	float erreur;						// Erreur sur l'accordage
	float autocorr[L_TAMPON];			// Résultat de l'autocorrélation

	// Application d'une fenêtre de Hanning
	for (i=0;i<L_TAMPON;i++) {
		bloc[i] = FENETRE_HAN[i]*bloc[i];
	}

	// Autocorrélation
	//
	// VOTRE **SOLUTION** D'AUTOCORRÉLATION PAR FFT REMPLACE LA PROCHAINE LIGNE!!!
	faireAutocorr_fft(bloc, autocorr);
	//faireAutocorr(bloc, autocorr);
	//
	//

	// Recherche de la fréquence fondamentale
	f0 = getF0(autocorr, frqDesiree);

	// S'il y a un signal à l'entrée (Test avec energie du signal)
	if (autocorr[0] > 1000000) { // NOTE : autocorr[0] correspond à l'énergie du signal

		// Calcul de l'erreur d'accordage sur une échelle logarithmique (cents)
		if (f0 != 0)	erreur = 1200*log10(f0/frqDesiree)/0.301;
		else erreur = -1200;
	}
	else 
		erreur = 0;

	// Ajustement des plages maximales
	if (erreur>1200)
		erreur = 1200;

	if (erreur<-1200)
		erreur = -1200;

	return erreur;
}

/************************************************************************
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        12 juin 2009
    Revisions:  
  DESCRIPTION : 
	 Effectue l'autocorrélation du tampon circulaire d'échantillons 
  ENTRÉE
	 bloc[] : bloc d'échantillons sur lequel l'autocorrélation sera 
	 effectuée.
  SORTIE
     resultat[] : Tableau 1xL_BLOC contenant le résultat de 
     l'autocorrélation.
***********************************************************************/
void faireAutocorr(float bloc[], float resultat[]) {

	int i;
	short delai;	// Délai pour l'autocorrélation

	// Pour tous les délais de l'autocorrélation
	for (delai=0; delai<L_TAMPON; delai++) {		

		resultat[delai] = 0;	// Initialisation

		// Balayage des éléments du tampon (somme de produits)
		for(i=0; i<L_TAMPON-delai; i++)	{
			resultat[delai] = resultat[delai] + bloc[i] * bloc[i+delai];
		}
	}

	return;
}

void faireAutocorr_fft(float bloc[], float resultat[])
{
    int i;
    float x_n[L_TAMPON*2];
    float X_Z[L_TAMPON*4];
    short index[32];

    // Initialisation du signal d'entrée
    for (i = 0; i < L_TAMPON; i++)
    {
        // Initialisation du signal d'entrée
        x_n[i] = bloc[i];
        // Ajout des zéros au signal d'entrée
        x_n[i+L_TAMPON] = 0;
    }

    // Autocorrélation par fft
    // Remplissez les cases paires du tableau de floats avec les valeurs de l'entrée et les cases impaires avec des 0.
    for (i = 0; i < L_TAMPON*2;i++)
    {
        X_Z[2*i] = x_n[i];
        X_Z[2*i+1] = 0.;
    }

    // Calculer la FFT
    DSPF_sp_cfftr2_dit(X_Z, w, L_TAMPON*2);

    // Inverse-bit
    bitrev_index(index, L_TAMPON*2);

    DSPF_sp_bitrev_cplx((double *)X_Z, index, L_TAMPON*2);

    // multiplication dans le domaine fréquentiel
    for (i = 0; i < L_TAMPON*4; i+=2)
    {
        X_Z[i] = X_Z[i]*X_Z[i] + X_Z[i+1]*X_Z[i+1];
        X_Z[i+1] = 0.;
    }

    // Calculer la iFFT
    // On utilise la méthode 1 en prenant le conjugué complexe
    DSPF_sp_cfftr2_dit(X_Z, w, L_TAMPON*2);

    // Reverse-bit
    DSPF_sp_bitrev_cplx((double *)X_Z, index, L_TAMPON*2);

    // Écriture du résultat, on enlève les valeurs imaginaires
    for (i=0; i<L_TAMPON; i++)
    {
        resultat[i] = X_Z[2*i]/(L_TAMPON*2);
    }
}

/************************************************************************
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        12 juin 2009
  Revisions:  
  DESCRIPTION : 
	 Recherche la fréquence fondamentale à partir du résultat de 
	 l'autocorrélation
  ENTRÉES : 
     *xx : Tableau 1xL_BLOC contenant le résultat de l'autocorrélation.
  ENTRÉE
	 tampon[] : Tampon circulaire sur lequel l'autocorrélation sera 
	 effectuée.
	 noNote : Numéro de la corde de guitare à accorder
	 fDesirer : Fréquence fondamentale désirée pour l'accordage
  RETOUR : 
     Fréquence fondamentale de la note jouée par le guitariste
***********************************************************************/
float getF0(float xx[], float fDesirer) {

	float f0;					// Fréquence fondamentale de sortie
	int delaiF0=1;				// Délai de la fréquence fondamentale et son amplitude
	float amp_MAX;				// Amplitude de l'autocorrélation de la fréquence fondamentale
	float f_hi;					// Fréquences maximum possible pour la corde sélectionnée
	int delaiMin;				// Délais minimum possible pour la corde sélectionnée
	int delai;

	// Variables pour limiter la recherche dans l'autocorrélation
	f_hi = fDesirer*2;
	delaiMin = (int)((float)FS/f_hi);

	// Recherche du délai de la fréquence fondamentale
	amp_MAX = 0;
	for (delai = delaiMin; delai<L_TAMPON ; delai++) {
		if (xx[delai] > amp_MAX) { /* Si l'amplitude courrante de l'autoccorélation est 
								      plus grande que les amplitudes précédentes */
			amp_MAX =  xx[delai];	
			delaiF0 = delai;  		// Enregistrement du délai actuel
		}
		else {}
	}

	// Calcul de la fréquence fondamentale si une note est jouée
	if (delaiF0 >= delaiMin)
		f0 = (float)FS/delaiF0; 	// Calcul de la fréquence fondamentale
	else
		f0 = 0;						// Aucune note ne semble être jouée

	return f0;	
} 

