
	.def _SubEntierNonSigne32bits
	.def _SubEntierSigne32bits
	.def _SubFlottant64bits


	.data


	.text

;;;;;;;;;;;;;;;Cas 12;;;;;;;;;;;;;;;
; Type de donn�e: Entier non sign�
; Format de donn�e (IN): 32 bits
; Format de donn�e (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_SubEntierNonSigne32bits
	.asmfunc

	LDW *A4++, A1   ;Chargement des paramettres (op�rants)
	NOP 4
	LDW *A4++, A2
	NOP 4

	SUBU A1, A2, A5:A4					 ;Soustraction
	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc


;;;;;;;;;;;;;;;Cas 22;;;;;;;;;;;;;;;
; Type de donn�e: Entier sign�
; Format de donn�e (IN): 32 bits
; Format de donn�e (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_SubEntierSigne32bits
	.asmfunc

	LDW *A4++, A1 		;Chargement des paramettres (op�rants)
	NOP 4
	LDW *A4++, A2
	NOP 4

	SUB A1, A2, A4 			;Soustraction

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc


;;;;;;;;;;;;;;;Cas 32;;;;;;;;;;;;;;;
; Type de donn�e: Entier sign�
; Format de donn�e (IN): 64 bits
; Format de donn�e (OUT): 64 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_SubFlottant64bits
	.asmfunc

	LDW *A4++, A2			;Chargement des paramettres (op�rants)
	NOP 4
	LDW *A4++, A3
	NOP 4

	LDW *A4++, B0		;Chargement des paramettres (op�rants)
	NOP 4
	LDW *A4++, B1
	NOP 4

	SUBDP  A3:A2, B1:B0, A5:A4			;Soustraction
	NOP 6

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc
