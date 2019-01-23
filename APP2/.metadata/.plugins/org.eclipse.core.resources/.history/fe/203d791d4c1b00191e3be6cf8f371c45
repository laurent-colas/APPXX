#include <stdio.h>
#define TAILLE 8
void MaFonctionASM();
int MaSousASM(int val1, int val2);
int AdditionSimple(int val1, int val2);
int AdditionTableau(int *TableauValeur, int NbVal);

int DemoTamponCirculaire(int *TableauValeur, int taille);


int Tableau2[] = {1,2,3,4};
#pragma DATA_ALIGN(Tableau2, 32);

//int AdditionTableau(int *TableauDeValeur);
//void FonctionLab1();

int main(void) {

    int res = 0;
    int res2 = 0;
    int param1 = 3;
    int param2 = 4;
    int listEntier[TAILLE] = {1,2,3,5,7,9};



    //res = AdditionSimple(param1, param2);
    res = AdditionTableau(listEntier, 6);

    res2 = DemoTamponCirculaire(Tableau2, TAILLE*2);

   //MaFonctionASM();
//   FonctionLab1();

    printf("Le résultat de %d + %d est %d", param1, param2, res);


    return 0;
}
