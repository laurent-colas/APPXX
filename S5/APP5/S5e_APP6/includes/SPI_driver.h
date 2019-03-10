/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier SPI_driver.h
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/

#pragma once
#ifndef _SPI_MODULE_
#define _SPI_MODULE_

/***************************************************************************
	Include headers :
***************************************************************************/


// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition


#include "SPI_Config.h"

/***************************************************************************
	Set EXTERN macro :
***************************************************************************/

#ifdef SPI_DRIVER_MODULE_IMPORT
	#define EXTERN  
#else 
	#define EXTERN extern
#endif

/***************************************************************************
	Constants declaration :
***************************************************************************/

//#define something something_else

/***************************************************************************
	Types declaration here :
***************************************************************************/

// typedef, struct, enum, union, etc.

/***************************************************************************
	Global variables declaration :
***************************************************************************/

// look at example for EXTERN pattern



/***************************************************************************		
	Function prototype :
***************************************************************************/

// Function description here ...
EXTERN void SPI_init(void);

// ajoutez vos prototype de fonction ici


void initMCBSP();
void masterSPIMCBSP();

#undef SPI_DRIVER_MODULE_IMPORT

#undef EXTERN

#endif // end of #ifndef _SPI_MODULE_

// end of SPI_driver.h
