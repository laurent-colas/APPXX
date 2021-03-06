/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier Audio_driver.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/

/***************************************************************************
	Include headers :
***************************************************************************/

#include "Audio_driver.h"
#include "SPI_driver.h"

/***************************************************************************
	Include Module Header :
***************************************************************************/

#define AUDIO_DRIVER_MODULE_IMPORT


/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern far void vectors();   // Vecteurs d'interruption

extern unsigned char output;
extern int input;
extern int reception_micro;
/****************************************************************************
	Private macros and constants :
****************************************************************************/


/****************************************************************************
	Private Types :
****************************************************************************/

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/

// Use static keyword here

/****************************************************************************
	Private functions :
****************************************************************************/

// these function can only be called by this .c
// Use static keyword

/****************************************************************************
	Public functions :
****************************************************************************/

// Function description here ...
void Audio_init(void)
{
    comm_intr(DSK6713_AIC23_FREQ_44KHZ,DSK6713_AIC23_INPUT_MIC);
}


/****************************************************************************
	ISR :
****************************************************************************/

interrupt void c_int11(void)
{

    if(DSK6713_DIP_get(0) == 0){
        SPI_Write(int2ulaw(input_sample()));
    }
    else{
        Uint8 data = input_sample() >> 8;
        SPI_Write(data);
    }
	return;

    reception_micro = 1;
    input = input_sample();
    output_sample(output);

}

// end of Audio_driver.c
