/*
 * main.c
 */

#include <stdio.h>
#include "C6713Helper_UdeS.h"
#include "LectureDIP.h"
#include "InteruptsASM.h"

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

// Addresse de EMIF CE1 pour acces aux LED et DIP switches
#define CPLD_USER_REG 0x90080000

// Addresses pour les configurations des timers
#define TIMER1_CTL  0x01980000
#define TIMER1_PRD  0x01980004

// Addresses pour les configuration des GPIO
#define GPEN_REG_GPIO   0x01B00000
#define GPDIR_REG_GPIO  0x01B00004
#define GPPOL_REG_GPIO  0x01B00024

// Addresses pour la mémoire l'EMIF et l'ADC
#define EMIF_ADC_CE3_REG 0xB0380000 // CE3 à 0xB0000000 + 0x00380000 (3584KB)
#define EMIF_CE3_REG 0xB0380000

// Définition les mask pour les DEL
#define MASK_LED3 0x08
#define MASK_LED2 0x04
#define MASK_LED1 0x02
#define MASK_LED0 0x01
#define MASK_INI 0x06

// Déclarations des fonctions pour la configuration des registres  de timers
void configAndStartTimer(void);
void configGP4In(void);

// Déclarations des fonctions pour la configuration des registres  de l'ADC
void Initwrite(void);
void read(void);
void stopListening(void);

// Déclarations des fonctions pour la configuration des registres des interuptions
void c_int4(void);
void c_int11(void);
void c_int15(void);

// Déclarations des variables pour l'état des DIP switches
unsigned short DIP3Stat;
unsigned short DIP0Stat;
unsigned short DIP1Stat;
unsigned short DIP2Stat;

// Déclarations du nombre d'echantillon pour 10sec
int nbEchantillon = 160000;

// Déclarations de variable d'incrément pour la logique du code
int tempor = 1;
int incrementBoucle;
int j = 0;
int nbSon = 0;
long i = 0;
short input_data;

// Déclaration des fonctions
void startTimer(void);
short ajustGain(unsigned short,unsigned short);

int main(void) {

// État initial  du programme pour bon fonctionnement
    enableAllInterrupts();
    incrementBoucle = 0;
    comm_intr();
    *(unsigned int*) CPLD_USER_REG &= ~MASK_LED1;




///////////// Boucle principale /////////////


    while(1) {
        DIP3Stat = ReadStatusDIP3(); // Lecture de l'état de la switch 3 du DSP

        // Condition en fonction de l'état de la switch 3 du DSP
        // Démarre l'enregistrement et flash les DEL slmt si la switch est baissée
        if(DIP3Stat == 1 && i == 0) {
            //*(unsigned int*) CPLD_USER_REG |= MASK_LED1;
            enableInterrupt4();
            configGP4In();
            startTimer();

        }

        // Condition pour un enregistrement de 10sec
        // Stop l'enregistrement si on enregiste depuis 10sec
        if (i >= nbEchantillon) {
            *(unsigned int*) CPLD_USER_REG &= ~MASK_LED1;
            *(unsigned int*) CPLD_USER_REG &= ~MASK_LED0;
            stopListening();
            j = 0;
        }

        DIP0Stat = ReadStatusDIP0(); // Lecture de l'état de la switch 0 du DSP

        // Condition en fonction de l'état de la switch 0 du DSP
        // Démarre la lecure si la switch est baissée
        if(DIP0Stat == 1 && loopindex == 0) {
            *(unsigned int*) CPLD_USER_REG |= MASK_LED1;
            i=0;
            enableInterruptSin();
            output_left_sample(0); //output sample value
            while (loopindex < nbEchantillon) {

                DIP1Stat = ReadStatusDIP1(); // Lecture de l'état de la switch 1 du DSP
                DIP2Stat = ReadStatusDIP2(); // Lecture de l'état de la switch 2 du DSP

                // Applique le bon gain ("volume") en fonction de l'état des 2 switchs
                ajustGain(DIP1Stat,DIP2Stat); // Voir la définission de la fonction pour la logique
            }
        }

        // Condition pour l'arret de la lecture  (fini la dernière lecture avant d'arreter)
        if (loopindex >= nbEchantillon) {
            disableInterruptSin();
            nbSon = 0;
            loopindex = 0;
        }
    }
}




///////////// Définitions de fonction déclarées en tête /////////////

// Permet les configurations pour la gestion de l'enregistrement
void startTimer(void) {
    enableInterruptVoice();
    configAndStartTimer();
    i++;
}


// Permet la configuration pour la stopper l'enregistrement
void stopListening(void) {
    *(unsigned int*) TIMER1_CTL &= ~0x0080;
}


// Permet l'écriture à l'ADC (Utilisé pour démarer la communication)
interrupt void c_int15(void) {
    Initwrite();
}


// Permet Permet l'interruption pour l'écriture à l'ADC (Utilisé pour démarer la communication)
void Initwrite(void) {
    *(unsigned int*) 0xA0000000 = 0x00000;
    *(unsigned int*) EMIF_ADC_CE3_REG = 0x0000;
}


// Permet la lecture à l'ADC
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


// Permet l'interruption pour la lecture à l'ADC
interrupt void c_int4(void) {
    read();
}


// configuration des registres du timer
void configAndStartTimer(void) {
    *(unsigned int*) TIMER1_CTL &= ~0x0080;
    *(unsigned int*) TIMER1_PRD = 2000;         //Calculé à 1758, mais ajusté à 2000 pour bien sonner
    *(unsigned int*) TIMER1_CTL |= 0x00000303;
    *(unsigned int*) TIMER1_CTL |= 0x000000C0;
}


// Permet l'interruption pour la lecture
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


// Configuration des registres du GPIO
void configGP4In(void) {
    *(unsigned int*) GPEN_REG_GPIO = (*(unsigned int*) GPEN_REG_GPIO) | 0x00000010; // Enable GP4
    *(unsigned int*) GPDIR_REG_GPIO = (*(unsigned int*) GPDIR_REG_GPIO) & 0xFFFFFFEF; // Set GP4 as input
    *(unsigned int*) GPPOL_REG_GPIO = (*(unsigned int*) GPPOL_REG_GPIO) | 0x00000010; //Set interrupt on falling edge
}


// Fonction pour l'agustement du gain ("volume") lors de la lecture
short ajustGain(unsigned short DIP1Stat, unsigned short DIP2Stat){
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

    return gain;
}

