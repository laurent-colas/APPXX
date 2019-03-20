;****************************************************************************************
; APPEL :
; short * direct1FIR_ASM(short *cPtr, short ech, const short h[], short *y);
;
; A4 : Adresse du pointeur sur l'échantillon précédent
; B4 : Valeur de l'échantillon courant
; A6 : Adresse du début du tableau de coefficients du filtre, h[]
; B6 : Adresse de l'échantillon de sortie
;
; Return : la nouvelle adresse de cPtr (A4)
;
; NB:
; 1) La longueur du filtre passe-haut, N = 63 (ordre = 62), est fixe (hardcoded)
;    et n'est donc pas passée comme paramètre à la fonction
; 2) Comme le mode d'adressage circulaire est employé pour le tampon, il n'est pas nécessaire
;    de spécifier l'adresse de début du tampon mais simplement l'adresse de l'échantillon
;    courant. En effet, quand le pointeur du tampon est incrémenté ou décrémenté, le
;    mode d'adressage circulaire force le pointeur à rester dans le tampon ("wrapping"). Il suffit simplement
;    de spécifier la bonne longueur avec l'AMR et de bien aligner le tampon lors de sa
;    déclaration dans le programme C avec "#pragma DATA_ALIGN"
;
;****************************************************************************************

		.def	_direct1FIR_ASM
_direct1FIR_ASM:

; Mémoriser le contenu de AMR 
			MVC	AMR, B5
			STW	B5,*--B15 

; Ajuster le registre AMR
			MVKL 0x00C08000, B9	; Utiliser B7 comme pointeur avec BK1 comme compteur vers
			MVKH 0x00C08000, B9	; Le tampon contient 64 échantillons de 16 bits chacuns, donc 128 octets (2 X 64)
			MVC B9, AMR

; Charger l'adresse de l'échantillon précédent du tampon dans B7
			MV A4, B7

; Écrire l'échantillon courant dans le tampon
			STH B4, *++B7

; Retourner l'adresse de l'échantillon courant
			MV B7, A4

; Longueur du filtre, N (ordre + 1, valeur impaire puisque c'est un passe-haut symmétrique)
			MVK 63, A1

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
;...
;...
;... CODE À AJOUTER...
;...
;...
; Enregistrer la sortie y[n]
			STH A11, *B6

; DEBUGGAGE: balancer l'échantillion d'entrée directement dans la sortie
			STH B4, *B6

; Rétablir le contenu de AMR
			LDW	*B15++,B5
			NOP	5
			MVC	B5,AMR

; Retourner l'adresse à la fonction qui a fait l'appel (return)
			B B3
			NOP 5

		.end
