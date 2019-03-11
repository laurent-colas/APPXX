
/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier principal Telephone.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/ 

/***************************************************************************
	Include headers :
***************************************************************************/

// Used modules headers
//#include "module_example.h"
//#include "SPI_driver.h"
#include "Audio_driver.h"


// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition
//#include <math.h>



/****************************************************************************
	Private macros and constants :
****************************************************************************/

//vos  #defines ou const int blablabla
//unique à ce fichier

/****************************************************************************
	Extern content declaration :
****************************************************************************/

// déclaration des contenus utilisés ici mais définis ailleurs
/*
extern far void vectors();   // Vecteurs d'interruption


/****************************************************************************
	Private Types :
****************************************************************************/

// type struct, enum , etc. definition here

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/

/****************************************************************************
	Main program private functions prototypes :
****************************************************************************/

//déclarer vos prototype de fonction ici

/****************************************************************************
	Main Program :
****************************************************************************/

void main()
{
	// initialisation des modules et des périphériques
	Audio_init();
	SPI_init();
	// Boucle infinie
	char data;
	while(1)
	{

	}
}

/****************************************************************************
	Main program private functions :
****************************************************************************/
  

/****************************************************************************
	Main program interrupt service routines (ISR) :
****************************************************************************/

interrupt void c_int04(void)
{


}

// end of Telephone.c
