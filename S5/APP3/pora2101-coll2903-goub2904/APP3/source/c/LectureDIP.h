/*
 * LectureDIP.h
 *
 *  Created on: Feb 5, 2019
 *      Author: berth
 */

#ifndef SOURCE_C_LECTUREDIP_H_
#define SOURCE_C_LECTUREDIP_H_

// Définition les mask pour les DIP switchs
#define MASK_GET_DIP3_BIT 0x00000080
#define MASK_GET_DIP2_BIT 0x00000040
#define MASK_GET_DIP1_BIT 0x00000020
#define MASK_GET_DIP0_BIT 0x00000010

#define CPLD_USER_REG 0x90080000

// Déclarations des fonctions
unsigned short ReadStatusDIP0(void);
unsigned short ReadStatusDIP1(void);
unsigned short ReadStatusDIP2(void);
unsigned short ReadStatusDIP3(void);


///////////// Fonctions permettant la lecture des états des DIP switchs /////////////


unsigned short ReadStatusDIP3(void) {
    unsigned short DIP_status;
    DIP_status = *(unsigned int *) CPLD_USER_REG;
    DIP_status = DIP_status & MASK_GET_DIP3_BIT;
    DIP_status = !(DIP_status >> 7);
    return DIP_status;
}

unsigned short ReadStatusDIP2(void) {
    unsigned short DIP_status;
    DIP_status = *(unsigned int *) CPLD_USER_REG;
    DIP_status = DIP_status & MASK_GET_DIP2_BIT;
    DIP_status = !(DIP_status >> 6);
    return DIP_status;
}

unsigned short ReadStatusDIP1(void) {
    unsigned short DIP_status;
    DIP_status = *(unsigned int *) CPLD_USER_REG;
    DIP_status = DIP_status & MASK_GET_DIP1_BIT;
    DIP_status = !(DIP_status >> 5);
    return DIP_status;
}

unsigned short ReadStatusDIP0(void) {
    unsigned short DIP_status;
    DIP_status = *(unsigned int *) CPLD_USER_REG;
    DIP_status = DIP_status & MASK_GET_DIP0_BIT;
    DIP_status = !(DIP_status >> 4);
    return DIP_status;
}


#endif /* SOURCE_C_LECTUREDIP_H_ */
