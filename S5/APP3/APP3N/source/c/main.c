/*
 * main.c
 */

#include <stdio.h>
#include "C6713Helper_UdeS.h"
#include "LectureDIP.h"


unsigned int  fs=DSK6713_AIC23_FREQ_16KHZ; //set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
unsigned short inputsource=DSK6713_AIC23_INPUT_MIC; //select input

#define LOOPLENGTH 8
#define BUFFERLENGTH 256
int loopindex = 0; //table index
int bufindex = 0; //buffer index
short sine_table[LOOPLENGTH]={0,707,1000,707,0,-707,-1000,-707};
int out_buffer[BUFFERLENGTH]; //output buffer
short gain = 10;

#define N 240000 //buffer size 300 secs

short buffer[N];
#pragma DATA_SECTION(buffer,".EXT_RAM") //buffer in ext memory

#define CPLD_USER_REG 0x90080000




#define MASK_LED3 0x08
#define MASK_LED2 0x04
#define MASK_LED1 0x02
#define MASK_LED0 0x01
#define MASK_INI 0x06

#define TIMER1_CTL  0x01980000
#define TIMER1_PRD  0x01980004

#define GPEN_REG_GPIO   0x01B00000
#define GPDIR_REG_GPIO  0x01B00004
#define GPPOL_REG_GPIO  0x01B00024


#define EMIF_ADC_CE3_REG 0xB0380000
#define EMIF_CE3_REG 0xB0380000

void configAndStartTimer(void);
void configGP4In(void);
void enableInterruptSin(void);
void disableInterruptSin(void);
void enableInterruptVoice(void);
//void disableInterruptVoice(void);
void enableInterrupt4(void);
void enableAllInterrupts(void);

//void configRead(void);
void Initwrite(void);
void read(void);
void stopListening(void);

void c_int4(void);
void c_int11(void);
void c_int15(void);


unsigned short DIP3Stat;
unsigned short DIP0Stat;
unsigned short DIP1Stat;
unsigned short DIP2Stat;

int tempor = 1;
int nbEchantillon = 160000;
int incrementBoucle;
int j = 0;
int nbSon = 0;
long i = 0;
short input_data;

void startTimer(void);

int main(void) {


    enableAllInterrupts();

    incrementBoucle = 0;
    comm_intr();

    *(unsigned int*) CPLD_USER_REG &= ~MASK_LED1;

    while(1) {

        DIP3Stat = ReadStatusDIP3();


        if(DIP3Stat == 1 && i == 0) {
            *(unsigned int*) CPLD_USER_REG |= MASK_LED1;
            enableInterrupt4();
            configGP4In();
            startTimer();

        }
        if (i >= nbEchantillon) {
            *(unsigned int*) CPLD_USER_REG &= ~MASK_LED1;
            *(unsigned int*) CPLD_USER_REG &= ~MASK_LED0;
            stopListening();
            j = 0;
            i = 0;
        }

        DIP0Stat = ReadStatusDIP0();
        if(DIP0Stat == 1 && loopindex == 0) {
            enableInterruptSin();
            output_left_sample(0); //output sample value
            while (loopindex < nbEchantillon) {
                DIP1Stat = ReadStatusDIP1();
                DIP2Stat = ReadStatusDIP2();
				
                if (DIP1Stat == 1 && DIP2Stat == 1) {
                    gain = 0;
                }
                else if (DIP1Stat == 0 && DIP2Stat == 1) {
                    gain = 10;
                }
                else if (DIP1Stat == 1 && DIP2Stat == 0) {
                    gain = 15;
                }
                else {
                    gain = 5;
                }
            }
        }
        if (loopindex >= nbEchantillon) {
            disableInterruptSin();
            loopindex = 0;
        }
    }
}

void startTimer(void) {
    enableInterruptVoice();
    configAndStartTimer();
}

void stopListening(void) {
    *(unsigned int*) TIMER1_CTL &= ~0x0080; //0xFFFFFF7F;
}

interrupt void c_int15(void) {
    Initwrite();
}

void Initwrite(void) {
    *(unsigned int*) 0xA0000000 = 0x00000;
    *(unsigned int*) EMIF_ADC_CE3_REG = 0x0000;
}

void read(void) {
    input_data = *(unsigned int*) EMIF_ADC_CE3_REG; //temporaire;
    input_data -= 512; // offset de 2V au microphone
    buffer[i++] = input_data;
    if (j==8000) {
        *(unsigned int*) CPLD_USER_REG ^= MASK_LED0;
        j = 0;
    }
    j++;
}


interrupt void c_int4(void) {
//    *(unsigned int*) CPLD_USER_REG ^= MASK_LED3;
    read();
    // CS, RD
}

void configAndStartTimer(void) {
    *(unsigned int*) TIMER1_CTL &= ~0x0080; //0xFFFFFF7F;
    *(unsigned int*) TIMER1_PRD = 2000; //1757;//2812500; //2812;
    *(unsigned int*) TIMER1_CTL |= 0x00000303;
    *(unsigned int*) TIMER1_CTL |= 0x000000C0;
}



interrupt void c_int11(void) {
    short out_sample;
    out_sample = buffer[loopindex++]*gain;
    output_left_sample(out_sample); //output sample value
    out_buffer[bufindex++] = out_sample; //store in buffer
    if (bufindex >= BUFFERLENGTH){
        bufindex = 0; //check end buffer
    }
    nbSon++;
    return;
}


void configGP4In(void) {
    *(unsigned int*) GPEN_REG_GPIO = (*(unsigned int*) GPEN_REG_GPIO) | 0x00000010; // Enable GP4
    *(unsigned int*) GPDIR_REG_GPIO = (*(unsigned int*) GPDIR_REG_GPIO) & 0xFFFFFFEF; // Set GP4 as input
    *(unsigned int*) GPPOL_REG_GPIO = (*(unsigned int*) GPPOL_REG_GPIO) | 0x00000010; //Set interrupt on falling edge
}

