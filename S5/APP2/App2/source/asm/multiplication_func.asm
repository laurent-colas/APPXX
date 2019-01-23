	.def _MpyEntierNonSigneOp32bitsRes64bits
	.def _MpyEntierSigneOp32bitsRes64bits
	.def _MpyFractionnaireOp32bitsRes64bits_Q7_24_Q15_16
	.def _MpyFlottant64bits

	.data


	.text

;;;;;;;;;;;;;;;Cas 12;;;;;;;;;;;;;;;
; Type de donnée: Entier non signé
; Format de donnée (IN): 32 bits
; Format de donnée (OUT): 64 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_MpyEntierNonSigneOp32bitsRes64bits
	.asmfunc

	MVKL 0x00000010, A1 ;Mettre lower 16bit du shift dans A1
	MVKH 0x00000010, A1 ;Mettre upper 16bit du shift dans A1

	LDW *A4++, A2 ;Load LSB du premier operand dans A2
	NOP 4
	LDW *A4++, A3 ;Load MSB du premier operand dans A2
	NOP 4

	MPYU A2, A3, A6 ; Multiply Unsigned 16 LSB by Unsigned 16 LSB
	NOP 1
	MV A6, A4 ;Least significant calc #1

	MPYHLU A2,A3, A7 ;Multiply Unsigned 16 MSB by Unsigned 16 LSB
	NOP 1

	SHR A7, A1, A11 ;shift right Unsigned 16 MSB by Unsigned 16 LSB
	SHL A7, 16, A10  ;shift left Unsigned 16 MSB by Unsigned 16 LSB

	MPYHLU A3, A2, A8 ;Multiply Unsigned 16 MSB by Unsigned 16 LSB
	NOP 1
	SHR A8, A1, A13 ;shift right Unsigned 16 MSB by Unsigned 16 LSB
	SHL A8, 16, A12 ;shift left Unsigned 16 MSB by Unsigned 16 LSB

	MPYHU A2, A3, A6 ;Multiply Unsigned 16 MSB by Unsigned 16 MSB
	NOP 1
	;MV A9, A6 ;deplacer resultat des 2 MSB dans A6

	ADDU A10, A4, A5:A4 ;Add LSB #1 to LSB #2
	ADDU A12, A5:A4, A5:A4 ;Add LSB #1 and 2 to LSB #3

	ADDU A6, A5, A7:A6
	ADDU A11, A7:A6, A7:A6 ;Add MSB #1 to MSB #2
	ADDU A13, A7:A6, A7:A6 ;Add MSB #1 and 2 to MSB #3

	MV A6, A5 ;Mettre MSB dans le bon registre de retour

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc


;;;;;;;;;;;;;;;Cas 22;;;;;;;;;;;;;;;
; Type de donnée: Entier signé
; Format de donnée (IN): 32 bits
; Format de donnée (OUT): 64 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_MpyEntierSigneOp32bitsRes64bits
	.asmfunc

	LDW *A4++, A2    ;Chargement des paramettres (opérants)
	NOP 4
	LDW *A4++, A3
	NOP 4

	MVKL 0x00000010, A1
	MVKH 0x00000010, A1

	MPY A2, A3, A6 ; Multiply Unsigned 16 LSB by Unsigned 16 LSB
	NOP 1
	MV A6, A4 ;Least significant calc #1

	MPYHL A2,A3, A7 ;Multiply Unsigned 16 MSB by Unsigned 16 LSB
	NOP 1

	SHR A7, A1, A11
	SHL A7, 16, A10

	MPYHL A3, A2, A8 ;Multiply Unsigned 16 MSB by Unsigned 16 LSB
	NOP 1
	SHR A8, A1, A13
	SHL A8, 16, A12

	MPYH A2, A3, A9 ;Multiply Unsigned 16 MSB by Unsigned 16 MSB
	NOP 1
	MV A9, A6

	ADD A10, A4, A5:A4 ;Add LSB #1 to LSB #2
	ADD A12, A5:A4, A5:A4 ;Add LSB #1 and 2 to LSB #3

	ADD A6,A5,A7:A6
	ADD A11, A7:A6, A7:A6 ;Add MSB #1 to MSB #2
	ADD A13, A7:A6, A7:A6 ;Add MSB #1 and 2 to MSB #3

	MV A6, A5

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc

;;;;;;;;;;;;;;;Cas 32;;;;;;;;;;;;;;;
; Type de donnée: Fractionnaire
; Format de donnée (IN): 32 bits
; Format de donnée (OUT): 64 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_MpyFractionnaireOp32bitsRes64bits_Q7_24_Q15_16
	.asmfunc

    LDW *A4++,A6       ;Chargement des paramettres (opérants)
	NOP 4
	LDW *A4++,A7
	NOP 4

	MPYID A6,A7,A5:A4     ;Multiplication
	NOP 9

	SHL A5,0,A5 ; On enleve le signe négatif en trop
	SHRU A4,31,A8
	ADD A5,A8,A5 ;On ajoute le bit de l'autre registre

	SHL A4,0,A4 ; On décale à gauche

    B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5

	.endasmfunc

;;;;;;;;;;;;;;;Cas 44;;;;;;;;;;;;;;;
; Type de donnée: Fractionnaire
; Format de donnée (IN): 64 bits
; Format de donnée (OUT): 64 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_MpyFlottant64bits
	.asmfunc

    LDDW *A4++,A7:A6    ;Chargement des paramettres (opérants)
	NOP 4
	LDDW *A4++,A9:A8
	NOP 4

	MPYDP A7:A6,A9:A8,A5:A4     ;Multiplication
	NOP 9

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc
