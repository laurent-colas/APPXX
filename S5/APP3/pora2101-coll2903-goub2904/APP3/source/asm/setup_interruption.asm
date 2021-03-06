
	.def _enableInterruptVoice
	.def _disableInterruptVoice
	.def _enableInterruptSin
	.def _disableInterruptSin
	.def _enableInterrupt4
	.def _enableAllInterrupts




_enableInterruptVoice
	.asmfunc

MASK_INTERRUPT_IE15	.set	0x00008000

	MVC IER, B5
	MVKL MASK_INTERRUPT_IE15, B4
	MVKH MASK_INTERRUPT_IE15, B4

	OR B4, B5, B5
	MVC B5, IER


	B B3
	NOP 5


	.endasmfunc



_disableInterruptVoice
	.asmfunc
MASK_DIS_INTERRUPT_IE15	.set	0x00008000


	MVC IER, B5
	MVKL ~MASK_DIS_INTERRUPT_IE15, B4
	MVKH ~MASK_DIS_INTERRUPT_IE15, B4

	AND B4, B5, B5
	MVC B5, IER


	B B3
	NOP 5


	.endasmfunc

_enableInterruptSin
	.asmfunc

MASK_INTERRUPT_IE11	.set	0x00000800

	MVC IER, B5
	MVKL MASK_INTERRUPT_IE11, B4
	MVKH MASK_INTERRUPT_IE11, B4

	OR B4, B5, B5
	MVC B5, IER


	B B3
	NOP 5


	.endasmfunc


_disableInterruptSin
	.asmfunc
MASK_DIS_INTERRUPT_IE11	.set	0x00000800


	MVC IER, B5
	MVKL ~MASK_DIS_INTERRUPT_IE11, B4
	MVKH ~MASK_DIS_INTERRUPT_IE11, B4

	AND B4, B5, B5
	MVC B5, IER


	B B3
	NOP 5


	.endasmfunc


_enableInterrupt4
	.asmfunc
MASK_INTERRUPT_IE4	.set	0x00000012

	MVC IER, B5
	MVKL MASK_INTERRUPT_IE4, B1
	MVKH MASK_INTERRUPT_IE4, B1

	OR B1, B5, B5
	MVC B5, IER


	B B3
	NOP 5


	.endasmfunc


_enableAllInterrupts
	.asmfunc
MASK_EN_NMI	.set	0x00000002

	MVC CSR, B5 ;Enable global interupt
	OR B5, 0x01, B5
	MVC B5, CSR

	MVC IER, B5
	MVKL MASK_EN_NMI, B4
	MVKH MASK_EN_NMI, B4
	OR B4, B5, B5
	MVC B5, IER

	B B3
	NOP 5

	.endasmfunc
