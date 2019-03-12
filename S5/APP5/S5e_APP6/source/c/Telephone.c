
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
#include "SPI_driver.h"
#include "Audio_driver.h"
#include "dsk6713.h"
#include"dsk6713_led.h"
#include"dsk6713_dip.h"

#include <csl.h>
#include <csl_gpio.h>
#include <csl_timer.h>
#include <csl_irq.h>
#include <csl_mcbsp.h>


// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition
#include <math.h>

/****************************************************************************
	Private macros and constants :
****************************************************************************/

//vos  #defines ou const int blablabla
//unique à ce fichier
#define MASK_CTL0 0x00000001

/****************************************************************************
	Extern content declaration :
****************************************************************************/

// déclaration des contenus utilisés ici mais définis ailleurs

extern far void vectors();   // Vecteurs d'interruption


/****************************************************************************
	Private Types :
****************************************************************************/

// type struct, enum , etc. definition here

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/


int output = 0x00;
unsigned int input = 0x0000;
int reception_micro = 0;
int reception_SPI = 0;
static GPIO_Handle lehandle;
int Led_Flip = 0;
int target = 0;
int LastTarget = 0;
int DIP1Stat_Last = 0;

// Use static keyword here

/****************************************************************************
	Main program private functions prototypes :
****************************************************************************/

//déclarer vos prototype de fonction ici

void init_ext_intr();
void controle_relais();

/****************************************************************************
	Main Program :
****************************************************************************/

void main()
{
    //initialisation des DIP et LED
    DSK6713_DIP_init();
    DSK6713_LED_init();
	// initialisation des modules et des périphériques
	Audio_init();
	SPI_init();
	init_ext_intr();
	// Boucle infinie
	while(1)
	{
	    SPI_run();
	    controle_relais();
	}
}

/****************************************************************************
	Main program private functions :
****************************************************************************/
void init_ext_intr()
{
    /* Initialisation de la pin 4*/
    lehandle = GPIO_open(GPIO_DEV0, GPIO_OPEN_RESET);
    GPIO_pinEnable(lehandle, GPIO_PIN4);
    GPIO_pinDirection(lehandle, GPIO_PIN4, GPIO_INPUT);
    GPIO_intPolarity(lehandle, GPIO_GPINT4, GPIO_FALLING);

    /* Initialisation de l'IRQ*/
    IRQ_setVecs(vectors);
    IRQ_globalEnable();
    IRQ_nmiEnable();
    IRQ_map(IRQ_EVT_EXTINT4,4);
    IRQ_reset(IRQ_EVT_EXTINT4);
    IRQ_enable(IRQ_EVT_EXTINT4);
}

void controle_relais()
{
    unsigned int DIP3Stat,DIP1Stat;
    DIP3Stat=DSK6713_DIP_get(3);
    DIP1Stat=DSK6713_DIP_get(1);
    if(DIP3Stat==1)
    {
        DSK6713_LED_on(2);
        DSK6713_LED_off(3);
        // Écriture de l'état du relais
        DSK6713_rset(DSK6713_DC_REG,  DSK6713_rget(DSK6713_DC_REG) | MASK_CTL0 );
    }
    else
    {
        DSK6713_LED_on(3);
        DSK6713_LED_off(2);
        // Écriture de l'état du relais
        DSK6713_rset(DSK6713_DC_REG,  DSK6713_rget(DSK6713_DC_REG) & ~MASK_CTL0 );
    }
    if (DIP1Stat != DIP1Stat_Last)
    {
        Led_Flip = 1;
    }
    DIP1Stat_Last = DIP1Stat;
}

/****************************************************************************
	Main program interrupt service routines (ISR) :
****************************************************************************/

// end of Telephone.c
