;;;;;;D�finition des fonction en assembleur;;;;;;
	.def _DivIncrementation
	.def _DivSubc
	.def _DivFlottant32bits

	.data


	.text

;;;;;;;;;;;;;;;Cas 1Y;;;;;;;;;;;;;;;
; Type de donn�e: Entier non sign�
; Format de donn�e (IN): 32 bits
; Format de donn�e (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_DivIncrementation
	.asmfunc

		;boucle avec un compteur(exemple 8*6, on additionne8+8=16 16+8=24..... On reproduit l'op�ration 6foisS)
    LDW *A4++,A5 ; load la premiere valeur dans le num�rateur dans le registre A5
    NOP 5
    LDW *A4++,A6 ; load la deuxi�me valeur dans le d�nominateur dans le registre A6
    NOP 5
    ZERO A7 ; on met le registre A7 � 0

	CMPLTU A5,A6,A1 ; on v�rifie si le num�rateur est inf�rieur  au d�nominateur (A5<A6?)
	MV A7,A4		; on met A4(registre pour la valeur de retour) � 0 comme �a si le d�nominateur est sup�rieur au num�rateur
	[A1] B end_boucle_division  ; si A5<A6 au d�nominateur on va la fin de la boucle
    NOP 5

boucle_division:
	;on ajoute une comparaison si on a A6>A5 on sort de la boucle
	CMPEQ A5,A6,A1  ; on v�rifie si A5=a6
	NOP 5
	CMPGTU A5,A6,A2 ; on �rifie si A5>A6
	NOP 5
	AND A1,A2,A1   ; On fait un and pour dire que le registre A1 vaut 1 si A1 et A2 valent 1 (les deux condition pr�c�dentes sont v�rifi�es)
	NOP 5
	[A1] B end_boucle_division 			; si A1=1 on sort de la boucle
	CMPLTU A5,A6,A2   ; A5<A6?
	[A2] B end_boucle_division  ; si A5<A6 on sort de la boucle
    SUB A5,A6,A5      ; on soustrait le d�nominateur et le num�rateur
    ADD A7,1,A7    ; on ajoute 1 au registre A7, ce registre est la valeur du quotient
    MV A5,A1 ; on attribut � A4(registre qui r�cup�re la valeur de retour) la valeur du resgistre A7 (quotient )
    NOP 5
    MV A7,A4    ; on retourne au d�but de la boucle
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

    LDW *A4++,A5  ; load la premiere valeur dans le num�rateur dans le registre A5
    NOP 5
    LDW *A4++,A6 ; load la deuxi�me valeur dans le d�nominateur dans le registre A6
    NOP 5
	ZERO A0  ; on met � 0 le registe A0

    CMPGTU A6,A5,A1  ;on compare le d�nominateur et le num�rateur (A6>A5?)
    NOP 5
    [A1] B Return_Zero		; on va dans le sous-programme qui retourne la valeur 0 si la condition pr�c�dente est v�rifi�e
	CMPEQ A0,A5,A1			; on verifie si le num�rateur vaut 0
	NOP 5
	[A1] B Return_Zero		; on va dans le sous-programme qui retourne la valeur 0 si la condition pr�c�dente est v�rifi�e
	CMPEQ A0,A6,A1			; on verifie si le d�nomnateur vaut 0
	NOP 5
	[A1] B Return_moins_un   ; on va dans le sous-programme qui retourne la valeur -1 si la condition pr�c�dente est v�rifi�e

	LMBD 1,A6,A0			; on v�rifie quel est le bit le plus � gauche au d�nominateur
	LMBD 1,A5,A2			; on v�rifie quel est le bit le plus � gauche pour le num�rateur
	SUBU A0,A2,A9:A8		; calcul de la valeur du d�calage  (shift)
	SHL A6,A8,A6			; fait un d�calage � gauche valant le (shift)

	;MV A8,A1
	MVK 0,A10  ; on attibut au registre A10 la valeur A10 (A10=10) pour la boucle for

BoucleFor:
	SUBC A5,A6,A5   ; on fait le subc
	ADD A10,1,A10
	;SUB A1,1,A1

	CMPGTU A8,A10,A1   ; on v�rifie si shift>i
	NOP 5
	[A1] B BoucleFor      ; condition pr�c�dente v�rifi�e on retourne au d�but de la boucle
	NOP 5
	CMPEQ A8,A10,A1   ; on v�rifie sur i==shift
	[A1] B BoucleFor       ; condition pr�c�dente v�rifi�e on retourne au d�but de la boucle
	NOP 5

	B Return_Fin     ; aucune conditions de v�rifi� on va � la valeur de retour

Return_Zero:    ;sous programme pour la valeur de retour qui vaut zero
	ZERO A4      ; A4(valeur de retour est mise � 0)
	NOP 5
	B FinSubC  ; on va directement � la fin du sous programme
	NOP 5

Return_Fin:   ; valeur de retour de fin
	ADD A8,1,A8      ; on fait shift+1 (comme dans le code C)
	MVK 32, A0

	SUBU A0,A8,A9:A8      ; on fait la soustraction 32-shift+1= d�calage de fin
	SHL A5,A8,A5         ; d�calage � gauche de la valeur obtenue en d�calage de fin
	SHRU A5,A8,A4      ; d�calage � droite de la valeur obtenue en d�calage de fin
	NOP 5
	B FinSubC        ; on va � la fin du programme subc
	NOP 5

Return_moins_un:				; sous programme de la valeur de retour valant -1
	MVK  -1,A4				; On attribut au registre de retour la valeur -1
	NOP 5
	B FinSubC				; on va � la fin du programme SubC
	NOP 5

FinSubC:
    B B3 					;INDISPENSABLE ; B3 contient l'adresse de retour
	NOP 5
    .endasmfunc


;;;;;;;;;;;;;;;Cas 42;;;;;;;;;;;;;;;
; Type de donn�e: Flottant
; Format de donn�e (IN): 32 bits
; Format de donn�e (OUT): 32 bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_DivFlottant32bits
	.asmfunc

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5
	.endasmfunc
