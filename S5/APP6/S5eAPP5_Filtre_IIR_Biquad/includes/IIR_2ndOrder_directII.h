/* main.c */
/*
  Créateur:    Bruno Gagnon, M. Sc.A
  Date:        16 juillet 2009
  Revisions:   

  DESCRIPTION : 
     Réalisation d'un filtre IIR biquad de forme direct type II
*/

#ifndef IIR_2NDORDER_DIRECTII_H_
#define IIR_2NDORDER_DIRECTII_H_

int IIR_2ndOrder_directII(int x, int w[], const short C[]);
int IIR_2ndOrder_directIIASM(int x, int w[], const short C[]);
int IIR_sat_25bits_ASM(int x);

#endif /* IIR_2NDORDER_DIRECTII_H */
