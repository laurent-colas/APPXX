/* findErrAccordage.c */
/*
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        12 juin 2009
  Revisions:   

  DESCRIPTION : 
	 Code donnant l'erreur en cents sur la note jou�e par rapport � 
	 la note nominale.

  ENTR�ES : 
	bloc[] : Tampon contenant les �hantillons audio utilis�s pour 
	         trouver l'erreur d'accordement

	frqDesirer : Fr�quence fondamentale nominale d�sir�e pour la corde

  RETOUR :
	Erreur d'accordage de la corde en cents

  NOTE : La cents repr�sente une distance entre deux fr�quences sur une �chelle 
         logarithmique. L'�quation est :
         
              DistCents = 1200*log2(f1/f2)
*/

#include <stdio.h>
#include <math.h>
#include "findErrAccordage.h"
#include "fenetreHanning.h" // Fen�tre de Hanning pr�-calcul�e
#include "CONSTANTES.h"		  // Constantes partag�es
// Includes pour la FFT
#include "twiddle_512.h"
#include "DSPF_sp_cfftr2_dit.h"
#include "DSPF_sp_bitrev_cplx.h"
#include "DSPF_sp_icfftr2_dif.h"


float findErrAccordage(float bloc[], float frqDesiree) {

	/*   NOTE : Le calcul de l'erreur d'accordage prend plus de temps 
	            que la dur�e d'une p�riode d'�chantillonnage. */

	int i;
	float f0;							// Fr�quence fondamentale de la note jou�e
	float erreur;						// Erreur sur l'accordage
	float autocorr[L_TAMPON];			// R�sultat de l'autocorr�lation

	// Application d'une fen�tre de Hanning
	for (i=0;i<L_TAMPON;i++) {
		bloc[i] = FENETRE_HAN[i]*bloc[i];
	}

	// Autocorr�lation
	//
	// VOTRE **SOLUTION** D'AUTOCORR�LATION PAR FFT REMPLACE LA PROCHAINE LIGNE!!!
	faireAutocorr_fft(bloc, autocorr);
	//
	//

	// Recherche de la fr�quence fondamentale
	f0 = getF0(autocorr, frqDesiree);

	// S'il y a un signal � l'entr�e (Test avec energie du signal)
	if (autocorr[0] > 1000000) { // NOTE : autocorr[0] correspond � l'�nergie du signal

		// Calcul de l'erreur d'accordage sur une �chelle logarithmique (cents)
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
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        12 juin 2009
    Revisions:  
  DESCRIPTION : 
	 Effectue l'autocorr�lation du tampon circulaire d'�chantillons 
  ENTR�E
	 bloc[] : bloc d'�chantillons sur lequel l'autocorr�lation sera 
	 effectu�e.
  SORTIE
     resultat[] : Tableau 1xL_BLOC contenant le r�sultat de 
     l'autocorr�lation.
***********************************************************************/
void faireAutocorr(float bloc[], float resultat[]) {

	int i;
	short delai;	// D�lai pour l'autocorr�lation

	// Pour tous les d�lais de l'autocorr�lation
	for (delai=0; delai<L_TAMPON; delai++) {		

		resultat[delai] = 0;	// Initialisation

		// Balayage des �l�ments du tampon (somme de produits)
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
    float real=0, imag=0;
    short index[32];

    // G�n�rez les valeurs d�index en utilisant la fonction bitrev_index()
    bitrev_index(index, L_TAMPON*2);

    // Initialisation du signal d'entr�e
    for (i = 0; i < L_TAMPON; i++)
    {
        x_n[i] = bloc[i];
    }
    // Ajout des z�ros au signal d'entr�e
    for (i = L_TAMPON; i < 2*L_TAMPON; i++)
    {
        x_n[i] = 0;
    }

    // Autocorr�lation par fft
    // Remplissez les cases paires du tableau de floats avec les valeurs de l'entr�e et les cases impaires avec des 0.
    for (i = 0; i < L_TAMPON*2;i++)
    {
        X_Z[2*i] = x_n[i];
        X_Z[2*i+1] = 0.;
    }

    // Calculer la FFT
    DSPF_sp_cfftr2_dit(X_Z, w, L_TAMPON*2);

    // Inverse-bit
    DSPF_sp_bitrev_cplx((double *)X_Z, index, L_TAMPON*2);

    // multiplication dans le domaine fr�quentiel
    for (i = 0; i < L_TAMPON*2; i++)
    {
        real = -X_Z[i];
        imag = -X_Z[i+1];

        X_Z[i] = real*real - imag*imag;
        X_Z[i] = 2*real*imag;
        //Xm[n] = sqrt(TableCos3[2*n]*TableCos3[2*n] + TableCos3[2*n+1]*TableCos3[2*n+1]);
    }

    // Reverse-bit
    DSPF_sp_bitrev_cplx((double *)X_Z, index, L_TAMPON*2);

    // Calculer la iFFT
    DSPF_sp_icfftr2_dif(X_Z, w, L_TAMPON*2);
}

/************************************************************************
  Cr�ateur:    Bruno Gagnon, M. Sc.A
  Date:        12 juin 2009
  Revisions:  
  DESCRIPTION : 
	 Recherche la fr�quence fondamentale � partir du r�sultat de 
	 l'autocorr�lation
  ENTR�ES : 
     *xx : Tableau 1xL_BLOC contenant le r�sultat de l'autocorr�lation.
  ENTR�E
	 tampon[] : Tampon circulaire sur lequel l'autocorr�lation sera 
	 effectu�e.
	 noNote : Num�ro de la corde de guitare � accorder
	 fDesirer : Fr�quence fondamentale d�sir�e pour l'accordage
  RETOUR : 
     Fr�quence fondamentale de la note jou�e par le guitariste
***********************************************************************/
float getF0(float xx[], float fDesirer) {

	float f0;					// Fr�quence fondamentale de sortie
	int delaiF0=1;				// D�lai de la fr�quence fondamentale et son amplitude
	float amp_MAX;				// Amplitude de l'autocorr�lation de la fr�quence fondamentale
	float f_hi;					// Fr�quences maximum possible pour la corde s�lectionn�e
	int delaiMin;				// D�lais minimum possible pour la corde s�lectionn�e
	int delai;

	// Variables pour limiter la recherche dans l'autocorr�lation
	f_hi = fDesirer*2;
	delaiMin = (int)((float)FS/f_hi);

	// Recherche du d�lai de la fr�quence fondamentale
	amp_MAX = 0;
	for (delai = delaiMin; delai<L_TAMPON ; delai++) {
		if (xx[delai] > amp_MAX) { /* Si l'amplitude courrante de l'autoccor�lation est 
								      plus grande que les amplitudes pr�c�dentes */
			amp_MAX =  xx[delai];	
			delaiF0 = delai;  		// Enregistrement du d�lai actuel
		}
		else {}
	}

	// Calcul de la fr�quence fondamentale si une note est jou�e
	if (delaiF0 >= delaiMin)
		f0 = (float)FS/delaiF0; 	// Calcul de la fr�quence fondamentale
	else
		f0 = 0;						// Aucune note ne semble �tre jou�e

	return f0;	
} 

