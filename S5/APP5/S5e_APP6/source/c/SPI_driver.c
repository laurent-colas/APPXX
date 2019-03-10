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
#include "dsk6713.h"


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


/****************************************************************************
	Private functions :
****************************************************************************/

// these function can only be called by this .c
// Use static keyword

/****************************************************************************
	Public functions :
****************************************************************************/


// Function description here ...
void initMCBSP()
{
    MCBSP_config(DSK6713_AIC23_CONTROLHANDLE,&MCBSP0_SPI_Cfg_Init);
}

void masterSPIMCBSP()
{
    MCBSP_config(DSK6713_AIC23_CONTROLHANDLE,&MCBSP0_SPI_Cfg_Master);
}

void SPI_Write(char data)
{
    while(!MCBSP_xrdy(DSK6713_AIC23_CONTROLHANDLE));
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, data);
}

char SPI_Read(void)
{
    while(!MCBSP_rrdy(DSK6713_AIC23_CONTROLHANDLE));
    return MCBSP_read(DSK6713_AIC23_CONTROLHANDLE);
}

void SPI_init(void)
{
    int CPLD_MISC_value;
    initMCBSP();
    masterSPIMCBSP();
    MCBSP_start(DSK6713_AIC23_CONTROLHANDLE,MCBSP_XMIT_START | MCBSP_RCV_START | MCBSP_SRGR_START | MCBSP_SRGR_FRAMESYNC, 256);
    // On assure l'écriture SPI vers le breadboard
    CPLD_MISC_value =  DSK6713_rget(DSK6713_MISC);
    CPLD_MISC_value ^= 0x01; // Le MCBSP0SEL est le bit 0;
    DSK6713_rset(DSK6713_MISC, CPLD_MISC_value);
	return;
}


/****************************************************************************
	ISR :
****************************************************************************/

// end of SPI_driver.c
