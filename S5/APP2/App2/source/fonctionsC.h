/*
 * fonctionsC.h
 *
 *  Created on: 18 janv. 2019
 *      Author: laure
 */

#ifndef SOURCE_FONCTIONSC_H_
#define SOURCE_FONCTIONSC_H_

#include <stdio.h>
#include <math.h>


// Entete des fonctions
unsigned int AddEntierNonSigne32bits(unsigned int *TabIntNoS);
int AddEntierSigne32bits(signed int *TabIntS);
int AddFractionnaire32bits_Q7_24_Q15_16(int *TabIntS);

unsigned int SubEntierNonSigne32bits(unsigned int *TabIntNoS);
int SubEntierSigne32bits(signed int *TabIntS);
double SubFlottant64bits(double *TabDouble);

unsigned long long int MpyEntierNonSigneOp32bitsRes64bits(unsigned int *TabIntNoS);
long long int MpyEntierSigneOp32bitsRes64bits(signed int *TabIntS);
long long int MpyFractionnaireOp32bitsRes64bits_Q7_24_Q15_16(signed int *TabIntS);
double MpyFlottant64bits(double *TabDouble);

unsigned int DivIncrementation(unsigned int *TabIntNoS);
unsigned int DivSubc(unsigned int *TabIntNoS);
int DivFlottant32bits(float *TabFloat);

int* EncrypterDonnees(int *TabDonnees, int nbTours);
int* Encrypt_en_c(int *TabDonnees, int nbtours);


void ObtenirType(int *Type);
void ObtenirFormat(int *Fromat);
void ObtenirOperation(int *Operation);

int ConvertirListe(int *Liste);

void ChoisirOperandesIntNoS(unsigned int *TabIntNoS);
void ChoisirOperandesIntS(signed int *TabIntS);
void ChoisirOperandesFrac(int *TabIntS);
void ChoisirOperandesFloat(float *TabFloat);
void ChoisirOperandesDouble(double *TabDouble);
//void ChoisirOperandesShortIntNoS(unsigned short int *TabShortIntNoS);
void ChoisirOperandesLongNoS(unsigned long *TabLongNoS);
//void ChoisirOperandesLLIntNoS(unsigned long long int *TabLLIntNoS);
void ChoisirDonnees(int *TabDonnees);

void AnalyserListe(int *Liste, unsigned int *TabIntNoS,  signed int *TabIntS, int *TabInt, float *TabFloat, double *TabDouble, unsigned long *TabLongNoS, int *TabDonnees);
int F_To_Q7_24(float input);
int F_To_Q15_16(float input);



// Declaration des fonctions

//Connaitre le choix de l�utilisateur sur le type de donn�es//
void ObtenirType(int *Type)
{
    printf("Quel est le type de donn�e?\n"
            "Entier non sign� -> 1\n"
            "Entier sign� -> 2\n"
            "Fractionnaire -> 3\n"
            "Flottant -> 4\n");
    scanf("%d", Type);
    printf("Vous avez rentrez %d. \n\n", *Type);
}

//Connaitre le choix de l�utilisateur sur le format des op�randes//
void ObtenirFormat(int *Format)
{
    printf("Quel est le format de donn�e?\n"
            "16 bits -> 1\n"
            "32 bits -> 2\n"
            "40 bits -> 3 (sauf pour division subc 32 bit)\n"
            "64 bits -> 4\n -->");
    scanf("%d", Format);
    printf("Vous avez rentrez %d. \n\n", *Format);
}

//Connaitre le choix de l�utilisateur sur l�op�ration//
void ObtenirOperation(int *Operation)
{
    printf("Quel est l'op�ration?\n"
            "Addition (+) -> 1\n"
            "Soustraction (-) -> 2\n"
            "Multiplication (x) -> 3\n"
            "Division (:) -> 4\n"
            "Chiffrer -> 5\n");
    scanf("%d", Operation);
    printf("Vous avez rentrez %d. \n\n", *Operation);
}

//Concat�ner l�information sur le type et le format de donn�es en une seule information//
int ConvertirListe(int *Liste) {
    int choix = Liste[0] * 10;
    choix = choix + Liste[1];
    return choix;
}

void AnalyserListe(int *Liste, unsigned int *TabIntNoS,  signed int *TabIntS, int *TabInt, float *TabFloat, double *TabDouble, unsigned long *TabLongNoS, int *TabDonnees) {
    unsigned int res;
    int resIntS;
    int choix = ConvertirListe(Liste);
    double resDouble;
    int i;
    float res4;
    double res3;
    unsigned long long int temp3;
    long long int temp4;

    if (Liste[2] == 1) {
        switch(choix) {
            case 12:
                ChoisirOperandesIntNoS(TabIntNoS);
                res = AddEntierNonSigne32bits(TabIntNoS);
                printf("Le resultat est de %u", res);
                break;
            case 22:
                ChoisirOperandesIntS(TabIntS);
                resIntS = AddEntierSigne32bits(TabIntS);
                printf("Le resultat est de %d", resIntS);
                break;
            case 32:
                ChoisirOperandesFrac(TabInt);
                resIntS = AddFractionnaire32bits_Q7_24_Q15_16(TabInt);
                res4 = (float) resIntS;
                printf("Le resultat est de %f", res4/pow(2,16));
                break;
            default :
                printf("Not supported operation");
        }
    }
    else if (Liste[2] == 2) {
        switch(choix) {
               case 12:
                   ChoisirOperandesIntNoS(TabIntNoS);
                   res = SubEntierNonSigne32bits(TabIntNoS);
                   printf("Le resultat est de %u", res);
                   break;
               case 22:
                   ChoisirOperandesIntS(TabIntS);
                   resIntS = SubEntierSigne32bits(TabIntS);
                   printf("Le resultat est de %d", resIntS);
                   break;
               case 44:
                   ChoisirOperandesDouble(TabDouble);
                   resDouble = SubFlottant64bits(TabDouble);
                   printf("Le resultat est de %lf", resDouble);
                   break;
               default :
                   printf("Not supported operation");

               }

    }
    else if (Liste[2] == 3) {
        switch(choix) {
               case 12:
                   ChoisirOperandesIntNoS(TabIntNoS);
                   temp3 = MpyEntierNonSigneOp32bitsRes64bits(TabIntNoS);
                   printf("Le resultat est de %llu", temp3);
                   break;
               case 22:
                   ChoisirOperandesIntS(TabIntS);
                   temp4 = MpyEntierSigneOp32bitsRes64bits(TabIntS);
                   printf("Le resultat est de %lld", temp4);
                   break;
               case 32:
                   ChoisirOperandesFrac(TabInt);
                   temp4 = MpyFractionnaireOp32bitsRes64bits_Q7_24_Q15_16(TabInt);
                   res3 = (double) temp4;
                  printf("Le resultat est de %lf", res3/pow(2,40));
                   break;
               case 44:
                   ChoisirOperandesDouble(TabDouble);
                   resDouble = MpyFlottant64bits(TabDouble);
                   printf("Le resultat est de %lf", resDouble);
                   break;
               default :
                   printf("Not supported operation");
               }

    }
    else if (Liste[2] == 4) {
        switch(choix) {
        case 12:
            ChoisirOperandesIntNoS(TabIntNoS);
            res = DivIncrementation(TabIntNoS);
            printf("Le resultat est de %d",res);
            break;
        case 13:
            ChoisirOperandesIntNoS(TabIntNoS);
            res=DivSubc(TabIntNoS);
            printf("Le resultat est de %d",res,"\n");
            break;
        case 42:
            ChoisirOperandesFloat(TabFloat);
            printf("C'etait optionelle donc on ne la pas fait");
            break;
        default:
            printf("Unsupported operation");
        }

    }
    else if (Liste[2] == 5) {
        switch(choix) {
        case 22:
            ChoisirDonnees(TabDonnees);
            i = 0;
            TabDonnees = EncrypterDonnees(TabDonnees, 1);
//            TabDonnees =  Encrypt_en_c(TabDonnees, 1);
            i = 1;
            printf("\n Les donnees encryptees sont: \n");
            for (i = 0; i<=7; i++) {
                printf("%d , ", TabDonnees[i] );
            }

            break;
        default:
            printf("Unsupported operation");
        }

    }
    else {
        printf("erreur");
    }
}

void ChoisirOperandesIntNoS(unsigned int *TabIntNoS) {
    printf("Entrer premiere operande entier non signee \n -->");
    scanf("%u", TabIntNoS);
    fflush(stdin);

    printf("Entrer deuxieme operande entier non signee\n -->");
    scanf("%u", TabIntNoS+1);
    fflush(stdin);

    printf("Vous avez entrez %u et %u \n", TabIntNoS[0], TabIntNoS[1] );
}

void ChoisirOperandesIntS(signed int *TabIntS) {
    printf("Entrer premiere operande entier signee \n -->");
    scanf("%d", TabIntS);
    fflush(stdin);

    printf("Entrer deuxieme operande entier signee\n -->");
    scanf("%d", TabIntS+1);
    fflush(stdin);

    printf("Vous avez entrez %d et %d \n", TabIntS[0], TabIntS[1] );
}

void ChoisirOperandesFrac(int *TabInt) {
    float input;

    printf("Entrez premiere operande fractionnaire de -128 a 127\n -->");
    scanf("%f", &input);
    fflush(stdin);
    TabInt[0] = F_To_Q7_24(input);

    printf("Entrez deuxieme operande fractionnaire de -32768 a 32767.99  \n -->");
    scanf("%f", &input);
    fflush(stdin);

    TabInt[1] = F_To_Q15_16(input);

    printf("Vous avez entrez %d et %d \n", TabInt[0], TabInt[1] );
}

int F_To_Q7_24(float input) {
    return pow(2,24) * input;
}

int F_To_Q15_16(float input) {
    return pow(2,16) * input;
}

void ChoisirOperandesDouble(double *TabDouble) {
    double temp;
    printf("Entrer premiere operande flottant \n -->");
    scanf("%lf", TabDouble);
    fflush(stdin);

    printf("Entrer deuxieme operande flottant \n -->");
    scanf("%lf", &temp);
    fflush(stdin);
    TabDouble[1] = temp;

    printf("Vous avez entrez %lf et %lf \n", TabDouble[0], TabDouble[1] );
}

void ChoisirOperandesLongNoS(unsigned long *TabLongNoS) {
    printf("Entrer premiere operande entier non signee 40 bit \n -->");
    scanf("%lu", TabLongNoS);
    fflush(stdin);

    printf("Entrer deuxieme operande entier non signee 40 bit \n -->");
    scanf("%lu", TabLongNoS+1);
    fflush(stdin);

    printf("Vous avez entrez %lf et %lf \n", TabLongNoS[0], TabLongNoS[1] );
}

void ChoisirOperandesFloat(float *TabFloat) {
    printf("Entrer premiere operande float 32 bit \n -->");
    scanf("%lu", TabFloat);
    fflush(stdin);

    printf("Entrer deuxieme operande float 32 bit \n -->");
    scanf("%lu", TabFloat+1);
    fflush(stdin);

    printf("Vous avez entrez %lf et %lf \n", TabFloat[0], TabFloat[1] );
}

void ChoisirDonnees(int *TabDonnees) {
    int i;
   // int reg_file_conf = 0x00020040;  //00000000000 00100 0000000001000000;

    for(i=0; i < 8; i =i +1) {
        printf("Entrer entier signee %d  \n -->", i);
        scanf("%d", TabDonnees+i);
        fflush(stdin);
    }

    printf("Vous avez entrez: ");
    for (i=0; i<8; i++) {
        printf(" %d ", TabDonnees[i]);
    }

    asm("   MVKL 0x00040040, B5");
    asm("   MVKH 0x00040040, B5");
    asm("   MVC B5, AMR");
}

int* Encrypt_en_c(int *TabDonnees, int nbtours) {
    int i, j;
    for (i = 0; i < nbtours; i++) {
        for (j = 0; j < 8; j++) {
            TabDonnees[j] = TabDonnees[j] & 0xFFFFFFFF;
        }
    }
    return TabDonnees;
}

#endif /* SOURCE_FONCTIONSC_H_ */
