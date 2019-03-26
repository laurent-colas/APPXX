	.def	_IIR_2ndOrder_directII_ASM
	.def	_IIR_sat_25bits_ASM
	.def 	_fonction_mtlp
	.def 	_fonction_shift_int


_IIR_2ndOrder_directII_ASM:
	.asmfunc

	; Protection de context de tout les registres utilises
	STW B5, *B15--[2]
	MVC AMR, B5
	STW B5, *B15--[2]
	;STW A0, *B15--[2]
	;STW A1, *B15--[2]
	STW A7, *B15--[2]
	STW A8, *B15--[2]
	STW A9, *B15--[2]
	STW A10, *B15--[2]
	STW A11, *B15--[2]
	STW A12, *B15--[2]
	STW A13, *B15--[2]
	STW B0, *B15--[2]
	STW B1, *B15--[2]
	STW B7, *B15--[2]
	STW B9, *B15--[2]


	;MVKL 0x00000010, A1
	;MVKH 0x00000010, A1

	MVKL 0x00FFFFFF, B7 ; 2^24 - 1
	MVKH 0x00FFFFFF, B7
	MVKL 0xFF000000, B9 ;-2^24
	MVKH 0xFF000000, B9

	; Sat 25 bits du signal d'entre
	; comparaison du signal avec 2^24-1
	; Si x > 2^24-1 alors B0 = 1 et Branch a over
	CMPGT A4, B7, B0
	[B0] B Over
	NOP 5

	; comparaison du signal avec -2^24
	; Si x < -2^24 alors B1 = 1 et Branch a under
	CMPLT A4, B9, B1
	[B1] B Under
	NOP 5

	; Si x pas sature Branch a End
	B End
	NOP 5

Over:
	; Si signal > 2^24-1 alors le signal prend la valeur de 2^24-1
	MV B7, A4
	B End
	NOP 5

Under:
	; Si signal < -2^24 alors le signal prend la valeur de -2^24
	MV B9, A4
	B End
	NOP 5

End:
	; Fin de la saturation
	; multiplication de a0*x(n)
	;lmem = (long)C[3]*(long)x;
	LDH *A6[3], A7
	NOP 4
	MV A4, A8
	MPYID A7, A8, A13:A12
	NOP 9

	; changer le format a 13Q13
	; memA = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A9


	; multiplication de a1*w(n-1)
	; lmem = (long)C[4]*(long)w[1];
	LDH *A6[4], A7
	LDW *B4[1], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	; changer le format a 13Q13
	;memB = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A10

	; a0*x(n)-a1*w(n-1)
	;Soustraire memA a memB = memC
	SUB A9, A10, A11

	; multiplication de a2*w(n-2)
	; lmem = (long)C[5]*(long)w[2];
	LDH *A6[5], A7
	LDW *B4[2], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	; changer le format a 13Q13
	;memB = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	; Mettre valeur dans memB (A10)
	MV A12, A10

	; a0*x(n)-a1*w(n-1)-a2*w(n-2)
	; memB = a2*w(n-2)
	; memC = a0*x(n)-a1*w(n-1)
	;Soustraire memC a memB = memA
	SUB A11, A10, A9


	; Verification du resultat memA sat_25 bits
	; comparaison du signal avec 2^24-1
	; Si memA > 2^24-1 alors B0 = 1 et Branch a Over1
	CMPGT A9, B7, B0
	[B0] B Over1
	NOP 5

	; comparaison du signal avec -2^24
	; Si memA < -2^24 alors B1 = 1 et Branch a Under1
	CMPLT A9, B9, B1
	[B1] B Under1
	NOP 5

	; Si memA pas sature Branch a End
	MV A9, A10
	B End1
	NOP 5

Over1:
	MV B7, A10
	B End1
	NOP 5

Under1:
	MV B9, A10
	B End1
	NOP 5

End1:

	; Enregistremet de la variable intermédiaire w[0]
	STW A10, *B4[0]

	; y(n) = b0*w(n)+b1*w(n-1)+b2*w(n-2)

	; multiplication de b0*w(n)
	; lmem = (long)C[0]*(long)w[0];
	LDH *A6[0], A7
	LDW *B4[0], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	; mettre en format 13Q13
	; memA = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A9

	; b1*w(n-1)
	; lmem = (long)C[1]*(long)w[1];
	LDH *A6[1], A7
	LDW *B4[1], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	; mettre en format 13Q13
	; memB = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A10

	; b0*w(n)+b1*w(n-1)
	; Additionner memA a memB = memC
	ADD A9, A10, A11

	; b2*w(n-2)
	; lmem = (long)C[2]*(long)w[2];
	LDH *A6[2], A7
	LDW *B4[2], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	; mettre en format 13Q13
	; memA = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A9

	; y(n)= b0*w(n)+b1*w(n-1)+b2*w(n-2)
	; memA = b2*w(n-2)
	; memC = b0*w(n)+b1*w(n-1)
	; Additionner memC a memA = y
	ADD A11, A9, A4

	; Décalage des variables intermédiaires w
	; w[2] = w[1];
	LDW *B4[1], B6
	NOP 4
	STW B6, *B4[2]
	; w[1] = w[0];
	LDW *B4[0], B6
	NOP 4
	STW B6, *B4[1]

	; Remise en context des registres utilises
	LDW *++B15[2], B9
	NOP 5
	LDW *++B15[2], B7
	NOP 5
	LDW *++B15[2], B1
	NOP 5
	LDW *++B15[2], B0
	NOP 5
	LDW *++B15[2], A13
	NOP 5
	LDW *++B15[2], A12
	NOP 5
	LDW *++B15[2], A11
	NOP 5
	LDW *++B15[2], A10
	NOP 5
	LDW *++B15[2], A9
	NOP 5
	LDW *++B15[2], A8
	NOP 5
	LDW *++B15[2], A7
	NOP 5
	;LDW *++B15[2], A1
	;NOP 5
	;LDW *++B15[2], A0
	;NOP 5
	LDW *++B15[2], B5
	NOP 5
	MVC B5, AMR
	LDW *++B15[2], B5
	NOP 5

	B B3
	NOP 5

	.endasmfunc


; Fonction test pour valider fonctionnement de la saturation
_IIR_sat_25bits_ASM:
	.asmfunc

	MVKL 0x00FFFFFF, B7 ; 2^24 - 1
	MVKH 0x00FFFFFF, B7
	MVKL 0xFF000000, B9 ;-2^24
	MVKH 0xFF000000, B9

	; Verification du resultat A4 sat_25 bits
	; comparaison du signal avec 2^24-1
	; Si A4 > 2^24-1 alors B0 = 1 et Branch a Over1
	CMPGT A4, B7, B0
	[B0] B Over_func
	NOP 5

	; comparaison du signal avec -2^24
	; Si A4 < -2^24 alors B1 = 1 et Branch a Under1
	CMPLT A4, B9, B1
	[B1] B Under_func
	NOP 5

	; Si A4 pas sature Branch a End
	B End_func
	NOP 5

Over_func:
	MV B7, A4
	B End_func
	NOP 5

Under_func:
	MV B9, A4
	B End_func
	NOP 5

End_func:

	B B3
	NOP 5

	.endasmfunc


; Fonction test pour valider la multiplication d'un short avec un int
_fonction_mtlp:
	.asmfunc

	MV B4, A7
	MV A4, A8
	MPYID A7, A8, A13:A12
	NOP 9

	MV A13, A5
	MV A12, A4

	B B3
	NOP 5
	.endasmfunc


; Fonction test pour decalage 13 bits
_fonction_shift_int
	.asmfunc
	SHR A5:A4, 13, A5:A4

	B B3
	NOP 5
	.endasmfunc
