/* 
 * File:   comm_temp_i2c.h
 * Author: laure
 *
 * Created on 7 janvier 2019, 14:56
 */

#ifndef COMM_TEMP_I2C_H
#define	COMM_TEMP_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* COMM_TEMP_I2C_H */


#include "plib/i2c.h"
#include "pconfig.h"

unsigned char i2c_comm(void)
{
unsigned char temperature, addr, cmd_byte, status;
	TRISBbits.RB0 =1;
	TRISBbits.RB1 =1;
	SSPADD = 0x31; // 100KHz, Fosc = ?? Mhz
	OpenI2C(MASTER,SLEW_OFF);
	cmd_byte=1; //start with measure
	StartI2C();


	// while ( SSPCON2bits.SEN ); // wait until start condition is over
	// already included in StartI2C()
	
	addr = 0x90; // adresse du TC74 modèle A0 en écriture
	status = WriteI2C(addr);
	
	while (SSPCON2bits.ACKSTAT); //Bit 6 (I2C_V4 routine does not ackn.
	IdleI2C(); // while ( ( SSPCON & 0x1F ) || ( SSPSTATbits.R_W ) )
	WriteI2C(0x00);
	
	while (SSPCON2bits.ACKSTAT); // wait for the acknowledge from slave
	IdleI2C();
	RestartI2C(); // keep the line and avoid others to grab the line
	
	while ( SSPCON2bits.RSEN ); // wait until re-start condition is over
	IdleI2C();
	
	addr = 0x90; // adresse du TC74 modèle A0 en lecture
	WriteI2C(addr);
	
	while (SSPCON2bits.ACKSTAT);
	IdleI2C();
	
	temperature = getcI2C();
	IdleI2C();
	RestartI2C();
	
	addr = 0x90; // adresse du TC74 modèle A0 en lecture
	WriteI2C(addr);
	
	while (SSPCON2bits.ACKSTAT);
	IdleI2C();
	temperature = getcI2C();
	IdleI2C();
	StopI2C();
	CloseI2C();
	
	return temperature;
}