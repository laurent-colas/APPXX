/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier module_example.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/



/***************************************************************************
	Include headers :
***************************************************************************/

//#include "something.h"

/***************************************************************************
	Include Module Header :
***************************************************************************/

#define EXAMPLE_MODULE_IMPORT
#include "module_example.h"


/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern far void vectors();  // Vecteurs d'interruption


/****************************************************************************
	Private macros and constants :
****************************************************************************/

// These defines are only valid is this .c

//#define something somethingelse

/****************************************************************************
	Private Types :
****************************************************************************/

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/

// Use static keyword here

//example :
static short i = 0;
static int* my_pointer = NULL ;
static Coordinates my_coordinates = {0,0};

/****************************************************************************
	Private functions :
****************************************************************************/

// these function can only be called by this .c
// use static keyword
static void myPrivateFunction() 
{
	if(i==0)
	{
		my_coordinates.x = 1;
		my_coordinates.y = 2;
	}

	return;
} 

/****************************************************************************
	Public functions :
****************************************************************************/

// Function description here ...
void myModule_init(void)
{
	myPrivateFunction();
	i++;
	my_pointer = (int*) &i;

	printf("Hello World !\n");
	printf("Pour enlever ce message, enlevez simplement le module exemple.\n");
	return;
}

// Function description here ...
void myModule_function1(void)
{
	// do something
	return;
} 

// Function description here ...
void myModule_function2(void)
{
	// do something
	return;
} 

/****************************************************************************
	ISR :
****************************************************************************/

/*
	interrupt void example()
	{
		// do something
		// clear int flag
		return;
	}
*/

// end of module_example.c
