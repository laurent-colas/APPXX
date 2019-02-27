/*
 * Interupts.h
 *
 *  Created on: Feb 5, 2019
 *      Author: berth
 */

#ifndef SOURCE_C_INTERUPTSASM_H_
#define SOURCE_C_INTERUPTSASM_H_


///////////// Déclarations des fonction de configuration des interuptions en ASM /////////////

void enableAllInterrupts(void);
void enableInterruptSin(void);
void disableInterruptSin(void);
void enableInterruptVoice(void);
void enableInterrupt4(void);


#endif /* SOURCE_C_INTERUPTSASM_H_ */
