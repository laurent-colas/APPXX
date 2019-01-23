;;;;;;Définition des fonction en assembleur;;;;;;
	.def _DivIncrementation
	.def _DivSubc
	.def _DivFlottant32bits

	.data


	.text

;;;;;;;;;;;;;;;Cas 1Y;;;;;;;;;;;;;;;
; Type de donnée: Entier non signé
; Format de donnée (IN): 32 bits
; Format de donnée (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_DivIncrementation
	.asmfunc

		;boucle avec un compteur(exemple 8*6, on additionne8+8=16 16+8=24..... On reproduit l'opération 6foisS)
    LDW *A4++,A5 ; load la premiere valeur dans le numérateur dans le registre A5
    NOP 5
    LDW *A4++,A6 ; load la deuxième valeur dans le dénominateur dans le registre A6
    NOP 5
    ZERO A7 ; on met le registre A7 à 0

	CMPLTU A5,A6,A1 ; on vérifie si le numérateur est inférieur  au dénominateur (A5<A6?)
	MV A7,A4		; on met A4(registre pour la valeur de retour) à 0 comme ça si le dénominateur est supérieur au numérateur
	[A1] B end_boucle_division  ; si A5<A6 au dénominateur on va la fin de la boucle
    NOP 5

boucle_division:
	;on ajoute une comparaison si on a A6>A5 on sort de la boucle
	CMPEQ A5,A6,A1  ; on vérifie si A5=a6
	NOP 5
	CMPGTU A5,A6,A2 ; on érifie si A5>A6
	NOP 5
	AND A1,A2,A1   ; On fait un and pour dire que le registre A1 vaut 1 si A1 et A2 valent 1 (les deux condition précédentes sont vérifiées)
	NOP 5
	[A1] B end_boucle_division 			; si A1=1 on sort de la boucle
	CMPLTU A5,A6,A2   ; A5<A6?
	[A2] B end_boucle_division  ; si A5<A6 on sort de la boucle
    SUB A5,A6,A5      ; on soustrait le dénominateur et le numérateur
    ADD A7,1,A7    ; on ajoute 1 au registre A7, ce registre est la valeur du quotient
    MV A5,A1 ; on attribut à A4(registre qui récupère la valeur de retour) la valeur du resgistre A7 (quotient )
    NOP 5
    MV A7,A4    ; on retourne au début de la boucle
    NOP 5
    [A1] B boucle_division
    NOP 5
end_boucle_division:
    STW A5, *A6--

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc


_DivSubc
    .asmfunc

    LDW *A4++,A5  ; load la premiere valeur dans le numérateur dans le registre A5
    NOP 5
    LDW *A4++,A6 ; load la deuxième valeur dans le dénominateur dans le registre A6
    NOP 5
	ZERO A0  ; on met à 0 le registe A0

    CMPGTU A6,A5,A1  ;on compare le dénominateur et le numérateur (A6>A5?)
    NOP 5
    [A1] B Return_Zero		; on va dans le sous-programme qui retourne la valeur 0 si la condition précédente est vérifiée
	CMPEQ A0,A5,A1			; on verifie si le numérateur vaut 0
	NOP 5
	[A1] B Return_Zero		; on va dans le sous-programme qui retourne la valeur 0 si la condition précédente est vérifiée
	CMPEQ A0,A6,A1			; on verifie si le dénomnateur vaut 0
	NOP 5
	[A1] B Return_moins_un   ; on va dans le sous-programme qui retourne la valeur -1 si la condition précédente est vérifiée

	LMBD 1,A6,A0			; on vérifie quel est le bit le plus à gauche au dénominateur
	LMBD 1,A5,A2			; on vérifie quel est le bit le plus à gauche pour le numérateur
	SUBU A0,A2,A9:A8		; calcul de la valeur du décalage  (shift)
	SHL A6,A8,A6			; fait un décalage à gauche valant le (shift)

	;MV A8,A1
	MVK 0,A10  ; on attibut au registre A10 la valeur A10 (A10=10) pour la boucle for

BoucleFor:
	SUBC A5,A6,A5   ; on fait le subc
	ADD A10,1,A10
	;SUB A1,1,A1

	CMPGTU A8,A10,A1   ; on vérifie si shift>i
	NOP 5
	[A1] B BoucleFor      ; condition précédente vérifiée on retourne au début de la boucle
	NOP 5
	CMPEQ A8,A10,A1   ; on vérifie sur i==shift
	[A1] B BoucleFor       ; condition précédente vérifiée on retourne au début de la boucle
	NOP 5

	B Return_Fin     ; aucune conditions de vérifié on va à la valeur de retour

Return_Zero:    ;sous programme pour la valeur de retour qui vaut zero
	ZERO A4      ; A4(valeur de retour est mise à 0)
	NOP 5
	B FinSubC  ; on va directement à la fin du sous programme
	NOP 5

Return_Fin:   ; valeur de retour de fin
	ADD A8,1,A8      ; on fait shift+1 (comme dans le code C)
	MVK 32, A0

	SUBU A0,A8,A9:A8      ; on fait la soustraction 32-shift+1= décalage de fin
	SHL A5,A8,A5         ; décalage à gauche de la valeur obtenue en décalage de fin
	SHRU A5,A8,A4      ; décalage à droite de la valeur obtenue en décalage de fin
	NOP 5
	B FinSubC        ; on va à la fin du programme subc
	NOP 5

Return_moins_un:				; sous programme de la valeur de retour valant -1
	MVK  -1,A4				; On attribut au registre de retour la valeur -1
	NOP 5
	B FinSubC				; on va à la fin du programme SubC
	NOP 5

FinSubC:
    B B3 					;INDISPENSABLE ; B3 contient l'adresse de retour
	NOP 5
    .endasmfunc


;;;;;;;;;;;;;;;Cas 42;;;;;;;;;;;;;;;
; Type de donnée: Flottant
; Format de donnée (IN): 32 bits
; Format de donnée (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_DivFlottant32bits
	.asmfunc

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc
