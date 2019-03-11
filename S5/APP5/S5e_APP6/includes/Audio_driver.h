/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier Audio_driver.h
**  Auteurs : berth
**  Date : < derniere modification >
********************************************************/

#pragma once
#ifndef _AUDIO_MODULE_
#define _AUDIO_MODULE_

/***************************************************************************
	Include headers :
***************************************************************************/


// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition



#include "C6713Helper_UdeS.h"
#include "dsk6713_dip.h"
#define DSK6713_AIC23_INPUT_MIC 0x0015


unsigned char int2ulaw(short linear);

/***************************************************************************
	Set EXTERN macro :
***************************************************************************/

#ifdef AUDIO_DRIVER_MODULE_IMPORT
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
EXTERN void Audio_init(void);

// ajoutez vos prototype de fonction ici

#undef AUDIO_DRIVER_MODULE_IMPORT

#undef EXTERN

#endif // end of #ifndef _AUDIO_MODULE_

// end of Audio_driver.h
