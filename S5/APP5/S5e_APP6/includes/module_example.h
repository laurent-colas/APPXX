/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier module_example.h
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/

#pragma once
#ifndef _MODULE_EXAMPLE_ // à modifier, donnez un nom unique
#define _MODULE_EXAMPLE_

/***************************************************************************
	Include headers :
***************************************************************************/

// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition


// #include "something.h"

/***************************************************************************
	Set EXTERN macro :
***************************************************************************/

#ifdef EXAMPLE_MODULE_IMPORT //modifier selon votre fichier, donnez un nom unique
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

/* example :  */

	typedef struct COORDINATES
	{
		int x;
		int y;
	}Coordinates;
	
	typedef enum VOLUME
	{
		FAIBLE = 10, MOYEN = 50, FORT = 100
	}Volume;


/***************************************************************************
	Global variables declaration :
***************************************************************************/



	EXTERN int myModuleInt
	#ifdef EXAMPLE_MODULE_IMPORT
		= 0
	#endif
	;

	EXTERN int* myModulePointer
	#ifdef EXAMPLE_MODULE_IMPORT
		= NULL
	#endif
	;
	
	// use volatile for interrupt flags
	EXTERN volatile bool myModuleFlag
	#ifdef EXAMPLE_MODULE_IMPORT
		= false
	#endif
	;

/***************************************************************************		
	Function prototype :
***************************************************************************/

// Function description here ...
EXTERN void myModule_init(void);

// Function description here ...
EXTERN void myModule_function1(void);

// Function description here ...
EXTERN void myModule_function2(void);

#undef EXAMPLE_MODULE_IMPORT

#undef EXTERN

#endif // end of #ifndef _MODULE_EXAMPLE_

// end of module_example.h
