/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier SPI_driver.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/


/***************************************************************************
	Include headers :
***************************************************************************/

//#include "SPI_Config.h"


/***************************************************************************
	Include Module Header :
***************************************************************************/

#define SPI_DRIVER_MODULE_IMPORT
#include "SPI_driver.h"
#include "SPI_Config.h"
#include "dsk6713_aic23.h"
#include "dsk6713_dip.h"
#include "dsk6713.h"
#include <stdbool.h>

#include <csl.h>
#include <csl_gpio.h>
#include <csl_timer.h>
#include <csl_irq.h>
#include <csl_mcbsp.h>


/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern far void vectors();   // Vecteurs d'interruption

extern MCBSP_Handle DSK6713_AIC23_CONTROLHANDLE;

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
extern int output;
extern unsigned int input;
extern int reception_micro;
extern int reception_SPI;
extern GPIO_Handle lehandle;

/****************************************************************************
	Private functions :
****************************************************************************/

// these function can only be called by this .c
// Use static keyword

/****************************************************************************
	Public functions :
****************************************************************************/


// Function description here ...
void SPI_Write(unsigned int data)
{
    while(!MCBSP_xrdy(DSK6713_AIC23_CONTROLHANDLE));
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, data | SPI_WRITE_DATA);
}

unsigned int SPI_Read(void)
{
    while(!MCBSP_rrdy(DSK6713_AIC23_CONTROLHANDLE));
    return MCBSP_read(DSK6713_AIC23_CONTROLHANDLE) & SPI_READ_DATA;
}

void SPI_init(void)
{
    int CPLD_MISC_value;
    MCBSP_config(DSK6713_AIC23_CONTROLHANDLE, &MCBSP0_SPI_Cfg);
    MCBSP_start(DSK6713_AIC23_CONTROLHANDLE, MCBSP_XMIT_START | MCBSP_RCV_START | MCBSP_SRGR_START | MCBSP_SRGR_FRAMESYNC, 256);

    // On assure l'écriture SPI vers le breadboard
    CPLD_MISC_value =  DSK6713_rget(DSK6713_MISC);
    CPLD_MISC_value ^= 0x01; // Le MCBSP0SEL est le bit 0;
    DSK6713_rset(DSK6713_MISC, CPLD_MISC_value);

    // On fait la configuration vers MAX3111 en write
    while(!MCBSP_xrdy(DSK6713_AIC23_CONTROLHANDLE));
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, SPI_WRITE_CONFIG);

	return;
}


void SPI_run(void)
{
    if (reception_SPI) // On a reçu du data par SPI
    {
        short data = 0;
        if(DSK6713_DIP_get(0) == 0) // Avec companding
        {
            data = (short) ulaw2int((char) SPI_Read());
        }
        else // Sans companding
        {
            data = ((short) SPI_Read()) << 8; // Centrer autour de 0
        }
        output = 0x00000000 | data;
        output |= output << 16;
        reception_SPI = 0;
    }
    if (reception_micro)
    {
        unsigned int data = 0;
        // On a du data à envoyer par SPI
        if(DSK6713_DIP_get(0) == 0) // Avec companding
        {
            data = int2ulaw((short) input) & 0x00000000FF;
        }
        else // Sans Companding
        {
            data = input>>24 & 0x00000000FF;
        }
        SPI_Write(data);
        reception_micro = 0;
    }

}


/****************************************************************************
	ISR :
****************************************************************************/
interrupt void c_int04(void)
{
    reception_SPI = 1;
}

// end of SPI_driver.c
