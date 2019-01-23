	.def _extensionSigne

	.data


	.text

_extensionSigne
	.asmfunc

	INTDP A4, A5:A4
	NOP 4

	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5


	.endasmfunc
