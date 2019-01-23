	;;;;;;Définition des fonction en assembleur;;;;;;
	.def _AddEntierNonSigne32bits
	.def _AddEntierSigne32bits
	.def _AddFractionnaire32bits_Q7_24_Q15_16

	.data


	.text

;;;;;;;;;;;;;;;Cas 12;;;;;;;;;;;;;;;
; Type de donnée: Entier non signé
; Format de donnée (IN): 32 bits
; Format de donnée (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_AddEntierNonSigne32bits
	.asmfunc

	LDW *A4++, A1 ;Load 1er operand dans A1
	NOP 4
	LDW *A4++, A2 ;Load 2ieme operand dans A2
	NOP 4

	ADDU A1, A2, A5:A4 ;Additionner les deux operands

 	B B3 ; INDISPENSABLE ; B3 contient ladresse de retour
    NOP 5
	.endasmfunc

;;;;;;;;;;;;;;;Cas 22;;;;;;;;;;;;;;;
; Type de donnée: Entier signé
; Format de donnée (IN): 32 bits
; Format de donnée (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_AddEntierSigne32bits
	.asmfunc
	LDW *A4++, A1 ;Load 1er operand dans A1
	NOP 4
	LDW *A4++, A2 ;Load 2ieme operand dans A2
	NOP 4

	ADD A1,A2,A4 ;Additionner les deux operands

	B B3 ; INDISPENSABLE ; B3 contient ladresse de retour
    NOP 5
	.endasmfunc

;;;;;;;;;;;;;;;Cas 32;;;;;;;;;;;;;;;
; Type de donnée: Fractionnaire
; Format de donnée (IN): 32 bits
; Format de donnée (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_AddFractionnaire32bits_Q7_24_Q15_16
	.asmfunc

	LDW *A4++, A1 ;Chargement des paramettres (opérants)
	NOP 4
	LDW *A4++, A2
	NOP 4

	SHR A1,8,A1 ;Alignement des puissances pour addition

	ADD A1, A2, A4 ;Addition et destination dans registres de retour

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5

	.endasmfunc
