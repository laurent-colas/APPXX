/*
 * SimpleInterrupt.c
 *
 *  Created on: 27 févr. 2019
 *      Author: laurent
 */

#include <csl.h>
#include <csl_timer.h>
#include <csl_irq.h>
#include <csl_gpio.h>
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition
#include <DSK6713_LED.h>
#include <dsk6713.h>

far void vectors();   // Vecteurs d'interruption

void init_ext_intr(void);
interrupt void c_int04(void);

static TIMER_Handle hTimer1;
static GPIO_Handle hGPIO4;
static Uint32 TimerEventId;
static Uint32 DIR_PIN4 = GPIO_INPUT;
static Uint32 DIR_PIN9 = GPIO_OUTPUT;
static Uint32 POL_PIN4 = GPIO_RISING;
static int cnt = 0;

int flag_led = 0;

void init_ext_intr(void) {

    hGPIO4 = GPIO_open(GPIO_DEV0, GPIO_OPEN_RESET);
    GPIO_pinEnable(hGPIO4, GPIO_PIN4);

    DIR_PIN4 = GPIO_pinDirection(hGPIO4, GPIO_PIN4, DIR_PIN4);
    GPIO_intPolarity(hGPIO4, GPIO_GPINT4, POL_PIN4);

    GPIO_pinEnable(hGPIO4, GPIO_PIN9);
    DIR_PIN9 = GPIO_pinDirection(hGPIO4, GPIO_PIN9, DIR_PIN9);
    GPIO_pinWrite(hGPIO4,GPIO_PIN9,0);

    IRQ_setVecs(vectors);     /* point to the IRQ vector table    */
    IRQ_globalEnable();       /* Globally enable interrupts       */
    IRQ_nmiEnable();          /* Enable NMI interrupt    */
    IRQ_map(IRQ_EVT_EXTINT4, 4);
    IRQ_reset(IRQ_EVT_EXTINT4);
    IRQ_enable(IRQ_EVT_EXTINT4);



}

interrupt void c_int04(void) {
    if(flag_led == 0){
        flag_led = 1;
    }
    else{
        flag_led = 0;
    }
}





void main() {

    CSL_init();
    init_ext_intr();
    DSK6713_init();
    DSK6713_LED_init();

    // Boucle infinie
    while(1)
    {
        if(flag_led == 1){
            GPIO_pinWrite(hGPIO4,GPIO_PIN9,1);
        }
        else{
            GPIO_pinWrite(hGPIO4,GPIO_PIN9,0);
        }
    }
}


