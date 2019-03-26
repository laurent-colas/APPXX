;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; Filtre FIR passe haut ;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Appel de la fonction : short * FIR_ASM (short *cPtr, short ech, const short h[], short *y);

	.def _FIR_ASM
	
	.data
	
	.text
	
	
_FIR_ASM
	.asmfunc
	
; Protection de contexte
	STW	A0,*B15--[2]
	STW	A1,*B15--[2]
	STW	A2,*B15--[2]
	STW	A5,*B15--[2]
	STW	A8,*B15--[2]
	STW A11,*B15--[2]

	STW	B3,*B15--[2]
	STW	B5,*B15--[2]
	STW	B7,*B15--[2]
	STW	B9,*B15--[2]

; Mémoriser le contenu de AMR
	MVC	AMR, B5
	STW	B5,*B15--[2]

; Ajuster le registre AMR
	MVKL 0x01008000, B9	; Utiliser B7 comme pointeur avec BK1 comme compteur vers
	MVKH 0x01008000, B9	; Le tampon contient 64 échantillons de 16 bits chacuns, donc 512 octets (2 X 256)
	MVC B9, AMR

; Charger l'adresse de l'échantillon précédent du tampon dans B7
	MV A4, B7

; Écrire l'échantillon courant dans le tampon
	STH B4, *++B7

; Retourner l'adresse de l'échantillon courant
	MV B7, A4

; Longueur du filtre, N (ordre + 1, valeur impaire puisque c'est un passe-haut symétrique)
	MVK 255, A1

; Initialisation y[n]=0
	ZERO A11;




;**********************************************************************
; Filtrage : boucle y[n] = y[n] + x[n-k]*h[k], pour k = 0..N, où N = 63
;            et où A11 est un accumulateur auquel on sont ajoutés les
;            produits x[n-k]*h[k] à chaque itération, k, de la boucle.
;            Le filtre n'est pas récursif...
;
;            ATTENTION: les x[n] sont contenus dans le tampon circulaire
;                       et sont donc loadés avec B7 et non A4.
;**********************************************************************


BoucleK
	SUB A1,1,A1 ;k=k-1
	|| LDH *--B7,B5 ;B5 = x[n-k]
	|| LDH *A6++, A5 ;A5 = h[k]
	NOP 4

	MPY A5,B5,B3 ;B3 = x[n-k]*h[k]
	NOP 1

	ADD B3, A11, A11 ;A11 (y[n]) = y[n] + x[n-k]*h[k]

	[A1] B BoucleK
	NOP 5

; Enregistrer la sortie y[n]
	SHR A11, 15, A11 ;Q30 -> Q15
	STH A11, *B6

; DEBUGGAGE: balancer l'échantillion d'entrée directement dans la sortie
	;STH B4, *B6

; Rétablir le contenu de AMR
	LDW	*++B15[2],B5
	NOP	5
	MVC	B5, AMR

; Restaurer le contexte
	LDW	*++B15[2],B9
	NOP	4
	LDW	*++B15[2],B7
	NOP	4
	LDW	*++B15[2],B5
	NOP	4
	LDW	*++B15[2],B3
	NOP	4
	LDW	*++B15[2],A11
	NOP	4
	LDW	*++B15[2],A8
	NOP	4
	LDW	*++B15[2],A5
	NOP	4
	LDW	*++B15[2],A2
	NOP	4
	LDW	*++B15[2],A1
	NOP	4
	LDW	*++B15[2],A0
	NOP	4

; Retourner l'adresse à la fonction qui a fait l'appel (return)
	B B3
	NOP 5

	.endasmfunc
