/* getNewCommand.c */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        8 juin 2009
  Revisions:   

  DESCRIPTION : 
	 Permet de détectée une DIP swtich appuyée comme un bonton.
	 La fonction utilise un algorithme d'anti-rebond.

  ENTRÉE/SORTIE : 
	 DIPno : Numéro de la DIP Switch appuyé

  RETOUR : 
  	 Flag qui vaut 1 s'il y a une nouvelle commande.
*/

#include <stdio.h>
#include <time.h>
#include "getNewCommand.h"
#include "dsk6713_dip.h"

const int L_ATTENTE = 2000; // Durée de l'attente pour l'algo. d'anti-rebond

int getNewCommand(int * DIPno) {

	int isNew;						// flag : 1 s'il y a une nouvelle commande
	static int isEnAttente = 0;		// pour indiquer si on attend (flag : 1 si on attend)
	static int count = 1;			// conteur pour la durée de l'attente
	static int mem = 1;				// état global précédent		

	int	isOFF_dip0 = DSK6713_DIP_get(0),	isOFF_dip1 = DSK6713_DIP_get(1),	// capture de l'état actuel
		isOFF_dip2 = DSK6713_DIP_get(2),	isOFF_dip3 = DSK6713_DIP_get(3); // des dip switchs

	int etat = (isOFF_dip0 && isOFF_dip1 && isOFF_dip2 && isOFF_dip3); // état actuel global

	isNew = 0;
	if ( isEnAttente ) {	// On attend quelques temps pour revérifier la commande
		count ++;

		if ( count == L_ATTENTE ) {	// Si attente terminée
			count = 0;	// remise du compteur à 0
		//	isEnAttente = 0;	// fin de l'attente

			// le numéro de la DIP Switch appuyée
			*DIPno = getDipAppuyee(isOFF_dip0, isOFF_dip1, isOFF_dip2, isOFF_dip3);
			
			if (*DIPno >= 0)	// Si une DIP d'appuyé
				isNew = 1;
			else {}
		}

	}
	else {}
	
	// si un changement d'état causé par un bonton appuyé
	if (mem == 1 && etat ==0) { 	     
		isEnAttente = 1;  // démarage d'une attente

	// si un changement d'état causé par un bonton relaché
	} else if (mem == 0 && etat ==1) { 
		isEnAttente = 0; // fin d'une attente
		count = 0;

	// aucun changement d'état
	} else {}

	mem = etat;

	return isNew;
}

/************************************************************
 DESCRIPTION : pour obtenir le numéro de la dip appuyée
************************************************************/
int getDipAppuyee(isOFF_dip0, isOFF_dip1, isOFF_dip2, isOFF_dip3) {

	int noDipSwitch = -1; 

	if (!isOFF_dip0) {
		noDipSwitch = 0;
	} else if (!isOFF_dip1)	{
		noDipSwitch = 1;
	} else if (!isOFF_dip2)	{
		noDipSwitch = 2;
	} else if (!isOFF_dip3)	{
		noDipSwitch = 3;
	} else	{}

	return noDipSwitch;
}


// AVEC UN TIMER

/*
#include <time.h>
#define D_ATTENTE 0.01	 // Durée en sec de l'attente pour l'algo. d'anti-rebond

int isNewCommand(int * DIPno)
{
	static time_t startTime;	// variable pour temps de début d'un bouton appuyé
	time_t curTime;		// variable pour le temps courrant
	float dif;			// variable pour différence de temps

	int isNew = 0;					// flag : 1 s'il y a une nouvelle commande
	static int isEnAttente = 0;		// pour indiquer si on attend (flag : 1 si on attend)
	static int MemOldEtat = 1;		// état global précédent		

	int	isOFF_dip0 = DSK6713_DIP_get(0),	isOFF_dip1 = DSK6713_DIP_get(1),	// capture de l'état actuel
		isOFF_dip2 = DSK6713_DIP_get(2),	isOFF_dip3 = DSK6713_DIP_get(3); // des dip switchs

	int etat = (isOFF_dip0 && isOFF_dip1 && isOFF_dip2 && isOFF_dip3); // état actuel global

	if ( isEnAttente )		// On attend quelques temps pour revérifier la commande
	{
		curTime = time (NULL);
		dif = difftime (curTime,startTime);

		if ( dif > D_ATTENTE )	// Si attente terminée
		{
			startTime = time (NULL); // enregistrement du temps courrant

			// le numéro de la DIP Switch appuyée
			*DIPno = getDipAppuye(isOFF_dip0, isOFF_dip1, isOFF_dip2, isOFF_dip3);
			
			if (*DIPno >= 0)	// Si une DIP d'appuyé
				isNew = 1;
			else {}
		}

	}
	else {}
	
	// si un changement d'état causé par un bonton appuyé
	if (MemOldEtat == 1 && etat ==0) { 	     
		isEnAttente = 1;  // démarage d'une attente
		startTime = time (NULL); // enregistrement du temps courrant

	// si un changement d'état causé par un bonton relaché
	} else if (MemOldEtat == 0 && etat ==1) { 
		isEnAttente = 0; // fin d'une attente

	// aucun changement d'état
	} else {}

	MemOldEtat = etat;

	return isNew;
}

// pour obtenir le numéro de la dip appuyé
int getDipAppuye(isOFF_dip0, isOFF_dip1, isOFF_dip2, isOFF_dip3)
{
	int noDipSwitch = -1; 

	if (!isOFF_dip0) {
		noDipSwitch = 0;
	} else if (!isOFF_dip1)	{
		noDipSwitch = 1;
	} else if (!isOFF_dip2)	{
		noDipSwitch = 2;
	} else if (!isOFF_dip3)	{
		noDipSwitch = 3;
	} else	{}

	return noDipSwitch;
}
*/

// AVEC UN COMPTEUR (SANS LE TEMPS)

