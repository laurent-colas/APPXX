

	.def	_IIR_2ndOrder_directII_ASM
	.def	_IIR_sat_25bits_ASM
	.def 	_fonction_mtlp
	.def 	_fonction_shift_int
	.data

sat_24 .int 16777216

	.text


_IIR_2ndOrder_directII_ASM:
	.asmfunc

	MVK 13, A14
	MVKL 0x00000010, A1
	MVKH 0x00000010, A1

	MVKL 0x00FFFFFF, B7 ; 2^24 - 1
	MVKH 0x00FFFFFF, B7
	MVKL 0xFF000000, B9 ;-2^24
	MVKH 0xFF000000, B9

	; Sat 25 bits
	CMPGT A4, B7, B0
	[B0] B Over
	NOP 5

	CMPLT A4, B9, B1
	[B1] B Under
	NOP 5

	B End
	NOP 5

Over:
	MV B7, A4
	B End
	NOP 5

Under:
	MV B9, A4
	B End
	NOP 5

End:
	;lmem = (long)C[3]*(long)x;
	LDH *A6[3], A7
	NOP 4
	MV A4, A8
	MPYID A7, A8, A13:A12
	NOP 9
	; memA = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A9

	; lmem = (long)C[4]*(long)w[1];
	LDH *A6[4], A7
	LDW *B4[1], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	;memB = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A10

	;Soustraire memA a memB = memC
	SUB A9, A10, A11

	; lmem = (long)C[5]*(long)w[2];
	LDH *A6[5], A7
	LDW *B4[2], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	;memB = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A10
	;Soustraire memC a memB = memA
	SUB A11, A10, A9

	; sat_25 bits memA = memB
	CMPGT A9, B7, B0
	[B0] B Over1
	NOP 5

	CMPLT A9, B9, B1
	[B1] B Under1
	NOP 5

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

	; lmem = (long)C[0]*(long)w[0];
	LDH *A6[0], A7
	LDW *B4[0], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	; memA = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A9

	; lmem = (long)C[1]*(long)w[1];
	LDH *A6[1], A7
	LDW *B4[1], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	; memB = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A10

	; Additionner memA a memB = memC
	ADD A9, A10, A11

	; lmem = (long)C[2]*(long)w[2];
	LDH *A6[2], A7
	LDW *B4[2], A8
	NOP 4
	MPYID A7, A8, A13:A12
	NOP 9

	; memA = (int)(lmem>>13);
	SHR A13:A12, 13, A13:A12
	MV A12, A9

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

	B B3
	NOP 5

	.endasmfunc


_IIR_sat_25bits_ASM:
	.asmfunc

	MVKH sat_24, A2
	MVKL sat_24, A2

	LDW *A2, A2
	NOP 4

	SUB A2, 1, B7
	CMPGT A4, B7, B0
	[B0] B Over_func
	NOP 5

	SUB 0, A2, B9
	CMPLT A4, B9, B1
	[B1] B Under_func
	NOP 5

	MV A4, A4
	B End_func
	NOP 5

Over_func:
	MV B7, A4
	B End
	NOP 5

Under_func:
	MV B9, A4
	B End
	NOP 5

End_func:
	B B3
	NOP 5

	.endasmfunc


_fonction_mtlp:
	.asmfunc

	MVKL 0x00000010, A1
	MVKH 0x00000010, A1
	MV B4, A7
	MV A4, A8
	MPYID A7, A8, A13:A12
	NOP 9

	MV A13, A5
	MV A12, A4

	B B3
	NOP 5
	.endasmfunc


_fonction_shift_int
	.asmfunc
	SHR A5:A4, 13, A5:A4


	B B3
	NOP 5
	.endasmfunc
