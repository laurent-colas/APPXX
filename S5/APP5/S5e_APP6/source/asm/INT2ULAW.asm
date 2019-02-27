*==========================================================================
*
*     TEXAS INSTRUMENTS, INC.
*
*     NAME INT2ULAW
*
*     Revision Date: 10/6/99
*
*     USAGE
*           This routine is C-callable and can be called as:
*
*           unsigned char int2ulaw(short linear);
*
*           linear = 14-bit, right-justified PCM, sign-extended to 16-bits
*                    an 8-bit unsigned right-justified value is returned
*
*           If the routine is not to be used as a C-callable function,
*           then you need to initialize all passed parameters since these
*           are assumed to be in registers as defined by the calling
*           convention of the compiler. (See the C compiler reference
*           guide.)
*
*     C CODE
*           This is the C equivalent of the assembly code without
*           restrictions.  Note that the assembly code is hand optimized.
*
*           unsigned char int2ulaw(short linear){
*              unsigned char i, sign, segment, quant;
*              unsigned short output, absol, temp;
*
*                 absol=abs(linear)+33;
*                 temp=absol;
*                 sign=(linear >= 0)?1:0;
*                 for(i=0;i<16;i++){
*                    output=temp&0x8000;
*                    if(output)break;
*                    temp<<=1;
*                 }
*                 segment=11-i;
*                 quant=(absol>>segment)&0x0F;
*                 segment––;
*                 segment<<=4;
*                 output=segment+quant;
*                 if(absol>8191) output=0x7F;
*                 if(sign)
*                    return output^=0xFF;
*                 else
*                    return output^=0x7F;
*           }
*
*     DESCRIPTION
*           u-law binary encoding table
*           Biased Linear abs(Input)    Segment Quantization
*           0 0 0 0 0 0 0 1 a b c d x   0 0 0   a b c d
*           0 0 0 0 0 0 1 a b c d x x   0 0 1   a b c d
*           0 0 0 0 0 1 a b c d x x x   0 1 0   a b c d
*           0 0 0 0 1 a b c d x x x x   0 1 1   a b c d
*           0 0 0 1 a b c d x x x x x   1 0 0   a b c d
*           0 0 1 a b c d x x x x x x   1 0 1   a b c d
*           0 1 a b c d x x x x x x x   1 1 0   a b c d
*           1 a b c d x x x x x x x x   1 1 1   a b c d
*
*           Input:  14-bit linear PCM value in register A4
*           Output: 8-bit logarithmic u-law value in register A4
*           Return: return address in register B3
*           Regs:   code utilizes registers A0-A4 and B0-B4
*
*     TECHNIQUES
*           This code yields the smallest cycle-count for one channel by
*           using as many resources in parallel as possible.  It can be
*           modified to process several channels whereby the aggregate
*           cycle-count will be greater, but the effective cycle-count per
*           channel will be less.  This code only utilizes fixed-point
*           instructions, and can be used by any member of the TMS320C6000
*           DSP family.
*
*     ASSUMPTIONS
*           The input is assumed to be a 14-bit, right-justified value that
*           is sign-extended to 16 bits.  The returned value is unsigned,
*           8 bits, and right-justified.  This code does not employ
*           software pipelining, so interrupts need not be disabled.
*
*     MEMORY NOTE
*           This code does not utilize data memory, but does occupy 24
*           words of program memory.  All computation is performed within
*           CPU registers A0-A4 and B0-B4.
*
*     CYCLES
*           7 cycles, 35.0 nanoseconds at 200MHz, regardless of input
*
*==========================================================================
        .global _int2ulaw
        .text
*** BEGIN Benchmark Timing ***
_int2ulaw:
        ABS    .L1   A4,         A0 ;take absolute value of input
||      CMPLT  .L2x  A4, 0,      B0 ;is input<0?
||      MVK    .S1   26,         A1 ;this is used for determining segment
||      MVK    .S2   0x1FFF-33,  B2 ;set saturation rail
 
        ADDK   .S1   33,         A0 ;add bias to abs(input)
||      B      .S2   B3             ;begin to exit subroutine
||      CMPGTU .L2x  A0,   B2,   B1 ;is abs(input)>rail?
        SHR    .S1   A0,   1,    A0 ;start determining quantization
||      MVK    .S2   25,         B2 ;need this for shift
||      LMBD   .L1   1,    A0,   A2 ;figure where ”1”s start for abs(input)
 
        SUB    .L1   A1,   A2,   A2 ;need this to determine quantize value
||      SUB    .L2x  B2,   A2,   B2 ;need this for segment value
||      MVK    .S1   0x7F,       A3 ;use for polarity switch or saturation
 
  [!B1] SHR    .S1   A0,   A2,   A0 ;set quantization value of abs(input)
||[!B1] SHL    .S2   B2,   4,    B4 ;position place segment value
||[B1]  MV     .L1   A3,         A4 ;if abs(input)>rail, use saturation
 
  [!B1] ADD    .L1x  B4,   A0,   A4 ;add quantization value to segment
||      MVK    .S2   0xFF,       B2 ;need this for polarity switch at end
 
  [B0]  XOR    .L1   A4,   A3,   A4 ;reverse polarity for negative input
||[!B0] XOR    .S1x  A4,   B2,   A4 ;reverse polarity for positive input
*** END Benchmark Timing ***