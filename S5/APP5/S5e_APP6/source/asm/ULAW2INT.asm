*==========================================================================
*
*     TEXAS INSTRUMENTS, INC.
*
*     NAME ULAW2INT
*
*     Revision Date: 10/6/99
*
*     USAGE
*           This routine is C-callable and can be called as:
*
*           int ulaw2int(unsigned char log);
*
*           log = 8-bit unsigned, right-justified logarithmic value
*           a 14-bit linear PCM value, sign–extended to 32 bits is returned
*
*           If the routine is not to be used as a C-callable function,
*           then you need to initialize all passed parameters since these
*           are assumed to be in registers as defined by the calling
*           convention of the compiler, (See the C compiler reference
*           guide.)
*
*     C CODE
*           This is the C equivalent of the assembly code without
*           restrictions.  Note that the assembly code is hand optimized.
*
*           int ulaw2int(unsigned char log){
*              unsigned char sign, segment;
*              unsigned short temp, quant;
*
*                 temp=log^0xFF;
*                 sign=(temp&0x80)>>7;
*                 segment=(temp&0x70)>>4;
*                 quant=temp&0x0F;
*                 quant<<=1;
*                 quant+=33;
*                 quant<<=segment;
*                 if(sign)
*                 return 33-quant;
*                         else
*                 return quant-33;
*           }
*
*     DESCRIPTION
*           u-law binary decoding table
*           Segment Quant.    Biased Linear abs(Output)
*           0 0 0   a b c d   0 0 0 0 0 0 0 1 a b c d 1
*           0 0 1   a b c d   0 0 0 0 0 0 1 a b c d 1 0
*           0 1 0   a b c d   0 0 0 0 0 1 a b c d 1 0 0
*           0 1 1   a b c d   0 0 0 0 1 a b c d 1 0 0 0
*           1 0 0   a b c d   0 0 0 1 a b c d 1 0 0 0 0
*           1 0 1   a b c d   0 0 1 a b c d 1 0 0 0 0 0
*           1 1 0   a b c d   0 1 a b c d 1 0 0 0 0 0 0
*           1 1 1   a b c d   1 a b c d 1 0 0 0 0 0 0 0
*
*           Input: 8-bit logarithmic u-law value in register A4
*           Output:  14-bit linear PCM value in register A4 (becomes Q31)
*           Return: return address in register B3
*           Regs:  code utilizes registers A0-A2,A4 and B0,B3
*
*     TECHNIQUES
*           This code yields the smallest cycle-count for one channel by
*           using as many resources in parallel as possible.  It can be
*           modified to process several channels whereby the aggregate
*           cycle–count will be greater, but the effective cycle-count per
*           channel will be less.  This code only utilizes fixed-point
*           instructions, and can be used by any member of the TMS320C6000
*           DSP family.
*
*     ASSUMPTIONS
*           The input is assumed to be an 8-bit, right-justified value that
*           is zero-extended to 16 bits.  The returned value is 14 bits,
*           right-justified, and sign-extended to 32 bits.  This code does
*           not employ the use of software pipelining, so interrupts need
*           not be disabled.
*
*     MEMORY NOTE
*           This code does not utilize data memory, but does occupy 16
*           words of program memory.  All computation is performed within
*           CPU registers A0–A2,A4 and B0,B3.
*
*     CYCLES
*           6 cycles, 30.0 nanoseconds at 200MHz, regardless of input
*==========================================================================
        .global _ulaw2int
        .text
*** BEGIN Benchmark Timing ***
 
_ulaw2int:
        NOT    .L1   A4,   A0       ;reverse polarity of input bits
||      EXTU   .S1   A4,24,31,   A1 ;extract sign bit
||      B      .S2   B3             ;begin to exit subroutine
 
        MVK    .S1   33,   A2       ;create bias for expansion
||      SHL    .S2x  A0,   1,    B0 ;left–shift quantization bits by one
 
        EXTU   .S1   A0,25,29,   A0 ;extract segment bits
||      EXTU   .S2   B0,27,27,   B0 ;extract quantization bits
 
        ADD    .S2x  B0,   A2,   B0 ;add bias to quantization bits
        SHL    .S1x  B0,   A0,   A4 ;linearize
 
  [!A1] SUB    .L1   A2,   A4,   A4 ;input was negative, remove bias/negate
||[A1]  SUB    .S1   A4,   A2,   A4 ;input was positive, just remove bias
*** END Benchmark Timing ***