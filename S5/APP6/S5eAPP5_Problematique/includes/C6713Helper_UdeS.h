/*
 * C6713Helper.h
 *
 *  Created on: 2013-10-18
 *      Author: Paul Charette
 */

#ifndef C6713HELPER_H_
#define C6713HELPER_H_

#include "dsk6713_aic23.h"

void comm_poll();
void CODEC_start();
void CODEC_stop();
void output_sample(int);
void output_left_sample(short);
void output_right_sample(short);
Uint32 input_sample();
short input_left_sample();
short input_right_sample();
void comm_intr();

#endif /* C6713HELPER_H_ */
