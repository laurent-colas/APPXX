/*
 * SimpleInterrupt.c
 *
 *  Created on: 27 févr. 2019
 *      Author: jgaud
 */

#include <csl.h>
#include <csl_gpio.h>
#include <csl_timer.h>
#include <csl_irq.h>
#include <dsk6713_led.h>
#include <SPI_Config.h>
#include <csl_mcbsp.h>


static GPIO_Handle lehandle;
int Global = 0;
void c_int4();

extern far void vectors();

void init_ext_intr()
{
    /* Initialisation de la pin 4*/
    lehandle = GPIO_open(GPIO_DEV0, GPIO_OPEN_RESET);
    GPIO_pinEnable(lehandle, GPIO_PIN4);
    GPIO_pinDirection(lehandle, GPIO_PIN4, GPIO_INPUT);
    GPIO_intPolarity(lehandle, GPIO_GPINT4, GPIO_RISING);

    /* Initialisation de la pin 9*/
    GPIO_pinEnable(lehandle, GPIO_PIN9);
    GPIO_pinDirection(lehandle, GPIO_PIN9, GPIO_OUTPUT);

    /* Initialisation de l'IRQ*/
    IRQ_setVecs(vectors);
    IRQ_globalEnable();
    IRQ_nmiEnable();
    IRQ_map(IRQ_EVT_EXTINT4,4);
    IRQ_reset(IRQ_EVT_EXTINT4);
    IRQ_enable(IRQ_EVT_EXTINT4);
}

interrupt void c_int4()
{
    Global ^=1;
}

void main()
{
    DSK6713_init();
    DSK6713_LED_init();
    init_ext_intr();
    while(1)
    {
        if (Global == 1)
        {
            GPIO_pinWrite(lehandle,GPIO_PIN9,1);
        }
        else
        {
            GPIO_pinWrite(lehandle,GPIO_PIN9,0);
        }

    }

}


