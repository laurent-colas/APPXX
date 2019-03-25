/*
 * IIR_2ndOrder_directII.h
 *
 *  Created on: 25 mars 2019
 *      Author: laure
 */

#ifndef INCLUDES_IIR_2NDORDER_DIRECTII_H_
#define INCLUDES_IIR_2NDORDER_DIRECTII_H_

int IIR_2ndOrder_directII(int x, int w[], const short C[]);
int IIR_2ndOrder_directIIASM(int x, int w[], const short C[]);
int IIR_sat_25bits_ASM(int x);
long fonction_mtlp(int x, short C);
int fonction_shift_int(long mem);




#endif /* INCLUDES_IIR_2NDORDER_DIRECTII_H_ */
