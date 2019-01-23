/*
 * File:   main_app.c
 * Author:  Laurent Colas coll2903
 *          Penelope Cullet culp3701
 *          Adrien Porsain pora2101
 *
 * Created on 7 janvier 2019, 13:33
 */
#include <xc.h>
#include <stdbool.h> /* For true/false definition */
#include <i2c.h>
#include "config_bits.h"
#include <usart.h>

#define _XTAL_FREQ 10000000

//The speed of your internal(or)external oscillator
#define USE_AND_MASKS
#define vref 5.00	

char adc_value;
unsigned char temp;

unsigned char input = 0x64;
unsigned char sec_input = 0x63;

unsigned char UART1Config =0, baud =0;
unsigned char MsgFromPIC[] ="\r\nYou typed :";
char MessageBuffer[200];
int i=0;
char out;
char sys_state;

void SetupClock(void);
void Delay1Second(void);
void inite_UART(void);
void __interrupt() Serial_interrupt(void); 
void init(void);
void configureADC(void);
void write_7seg(unsigned char);
void closeADC(void);
void set_LED_ready(char);
char get_adc_value(void);
void send_volt_value(char);
void d_touch(void);
void f_touch(void);
void c_touch(void);

void SetupClock(){
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
}

void inite_UART() {
    SetupClock(); 
    // Internal Clock to 8MHz
    // RCSTAbits.SPEN = 1;
    TRISCbits.RC6 = 0;
    //TX pin set as output
    TRISCbits.RC7 = 1;
    //RX pin set as input
    SPBRG = 64; //107;//64;
    BAUDCONbits.TXCKP = 0;
    BAUDCONbits.RXDTP = 0;
    
    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.SENDB = 0;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TX9D = 0;
//    TXSTA = 0x24; //0b00100100; //0x24; // //20;
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    RCSTAbits.ADDEN = 0;
    RCSTAbits.RX9D = 0;
//    RCSTA = 0x90;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    PIE1bits.TXIE = 0;
    
}

char check(char input) {
    if(RCSTAbits.FERR == 1 || RCSTAbits.OERR == 1){
        write_7seg(0x79); // E
        CREN = 0;
        return 0x65;
    }
    if (input == 0x64) {
        write_7seg(0x5E); // d
        return 0x64;
    }
    if (input == 0x63) {
        write_7seg(0x39); // C
        return 0x63;
    }
    if (input == 0x66) {
        write_7seg(0x71); // F
        return 0x66;
    }
    else{
//        write_7seg(0x3E); // U
        return 0x75;
    }
}

void __interrupt() Serial_interrupt() {
    while(RCIF == 0);
    out = RCREG;
    sys_state = check(out);
}

void init() {
    // Initialisation de l'horloge Timer0 
    TMR0L = 0xFF;
    TMR0H = 0xFF;
    T0CON = 0x05;
    T0CONbits.TMR0ON=1;	// démarrage de l'horloge
    
    TRISCbits.RC1 = 0;
    PORTCbits.RC1 = 0;

    inite_UART();
}

void write_7seg(unsigned char c) {
    LATD = c;
}

void configureADC() {
    // Conversion Analogique a digital pin setup
    ADCON1bits.PCFG = 0b1101; // 0b1110; //0b1101; // Configure analog pins
    ADCON0bits.CHS0 = 1; // Selects channel 1 (AN1)
    
    ADCON2bits.ADFM = 0; //1; 0 = left justify
    ADCON2bits.ACQT = 0x6; // 0b110; // A/D Acquisition Time Select bits
    ADCON2bits.ADCS = 0x2; //0b010; // FOSC/32 A/D Conversion Clock Select bits
    
    ADCON0bits.ADON = 1; // A/D converter module is enabled

    // Configure A/D interupt
    
}

void closeADC() {
    ADCON0bits.ADON = 0; // A/D converter module is disabled
}

void set_LED_ready(char state) {
    LATCbits.LATC1 = state;
}

char get_adc_value() {
    char digi;
    // Start conversion 
    ADCON0bits.GO_DONE = 1;
    while(ADCON0bits.GO_DONE==1);
    digi = ADRESH;
    return digi;
}

void send_volt_value(char voltage) {
    while(TXIF==0);
    TXREG = voltage; //out;
}

void d_touch() {
    configureADC();
    set_LED_ready(1);
}

void f_touch() {
    closeADC();
    set_LED_ready(0);
}

void c_touch() {
    adc_value =  get_adc_value();
    send_volt_value(adc_value);
}

void main(void) {
    init();
    TRISDbits.RD0  = 0; //pin 0 du port D en sortie
    PORTDbits.RD0 = 0;  // pin 0 du portD à 0
    
    TRISDbits.RD1  = 0; //pin 0 du port D en sortie
    PORTDbits.RD1 = 0;  // pin 0 du portD à 0
    
    TRISDbits.RD2  = 0; //pin 0 du port D en sortie
    PORTDbits.RD2 = 0;
    
    TRISDbits.RD3  = 0; //pin 0 du port D en sortie
    PORTDbits.RD3 = 0;
    
    TRISDbits.RD4  = 0; //pin 0 du port D en sortie
    PORTDbits.RD4 = 0;
    
    TRISDbits.RD5  = 0; //pin 0 du port D en sortie
    PORTDbits.RD5 = 0;
    
    TRISDbits.RD6  = 0; //pin 0 du port D en sortie
    PORTDbits.RD6 = 0;
    
    TRISDbits.RD7  = 0; //pin 0 du port D en sortie
    PORTDbits.RD7 = 0;
       
    while(true){
        if (sys_state == 'd') {
            d_touch(); 
            while(sys_state!=0x66) {
                if (sys_state == 0x63) {
                    c_touch();
                    sys_state = 0x00;
                }
            }
            f_touch();
            sys_state = 0x00;    
        }
    }  
}
