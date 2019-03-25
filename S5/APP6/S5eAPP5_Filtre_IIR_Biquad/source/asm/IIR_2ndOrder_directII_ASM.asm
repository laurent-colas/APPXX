

	.def	_IIR_2ndOrder_directII_ASM
	.def	_IIR_sat_25bits_ASM

	.data

sat_24 .int 16777216

	.text


_IIR_2ndOrder_directII_ASM:
	.asmfunc

	MVK 13, A14

	LDW *A6[3], A7
	NOP 4
	INTDP A7, A1:A0
	NOP 4

	INTDP A4, A3:A2
	NOP 4
	;lmem
	MPYDP A1:A0, A3:A2, A13:A12
	NOP 9

	; memA
	DPINT A13:A12, A9
	NOP 3
	SHR A14, A9,  A9



	LDW *A6[4], A7
	NOP 4
	INTDP A7, A1:A0
	NOP 4

	LDW *B4[1], A7
	NOP 4
	INTDP A7, A3:A2
	NOP 4
	; lmem
	MPYDP A1:A0, A3:A2, A13:A12
	NOP 9

	; memB
	DPINT A13:A12, A10
	NOP 3
	SHR A14, A10, A10


	; memC
	SUB A9, A10, A11


	;C[5]
	LDW *A6[5], A7
	NOP 4
	INTDP A7, A1:A0
	NOP 4

	; w[2]
	LDW *B4[2], A7
	NOP 4
	INTDP A7, A3:A2
	NOP 4
	; lmem
	MPYDP A1:A0, A3:A2, A13:A12
	NOP 9

	; memB
	DPINT A13:A12, A10
	NOP 3
	SHR A14, A10, A10


	; memA
	SUB A11, A10, A9

	; sat_25 bits

	; w[0]
	STW A10, *B4[0]

	;lmem
	LDW *A6[0], A7
	NOP 4
	INTDP A7, A1:A0
	NOP 4

	LDW *B4[0], A7
	NOP 4
	INTDP A7, A3:A2
	NOP 4
	; lmem
	MPYDP A1:A0, A3:A2, A13:A12
	NOP 9

	; memA
	DPINT A13:A12, A9
	NOP 3
	SHR  A14, A9, A9



	;lmem
	LDW *A6[1], A7
	NOP 4
	INTDP A7, A1:A0
	NOP 4

	LDW *B4[1], A7
	NOP 4
	INTDP A7, A3:A2
	NOP 4
	; lmem
	MPYDP A1:A0, A3:A2, A13:A12
	NOP 9

	; memB
	DPINT A13:A12, A10
	NOP 3
	SHR A14, A10, A10


	; memC
	ADD A9, A10, A11


	;lmem
	LDW *A6[2], A7
	NOP 4
	INTDP A7, A1:A0
	NOP 4

	LDW *B4[2], A7
	NOP 4
	INTDP A7, A3:A2
	NOP 4
	; lmem
	MPYDP A1:A0, A3:A2, A13:A12
	NOP 9

	; memA
	DPINT A13:A12, A9
	NOP 3
	SHR A14, A9, A9


	; y
	ADD A11, A9, A4


	LDW *B4[1], B6
	NOP 4
	STW B6, *B4[2]

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
	[B0] B Over
	NOP 5

	SUB 0, A2, B9
	CMPLT A4, B9, B1
	[B1] B Under
	NOP 5

	MV A4, A4
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
	B B3
	NOP 5

	.endasmfunc
