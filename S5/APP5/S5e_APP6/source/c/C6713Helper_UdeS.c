#include <stdio.h>
//#include <math.h>
#include <dsk6713.h>
#include "C6713Helper_UdeS.h"

extern far void vectors(); //external function
// Uint16 inputsource; //= DSK6713_AIC23_INPUT_LINE;
// extern Uint32 fs= DSK6713_AIC23_FREQ_8KHZ;  //set sampling rate

#define using_bios
//#define DSK6713_AIC23_INPUT_LINE 0x0011
//#define DSK6713_AIC23_INPUT_MIC 0x0015
#define LEFT  1                  //data structure for union of 32-bit data
#define RIGHT 0                  //into two 16-bit data

static Uint32 CODECEventId, poll;

union {
	Uint32 uint;
	short channel[2];
	} AIC_data;


DSK6713_AIC23_Config config = { \
0x0017,  /* 0 DSK6713_AIC23_LEFTINVOL  Left line input channel volume */ \
0x0017,  /* 1 DSK6713_AIC23_RIGHTINVOL Right line input channel volume */\
0x0179,  /* 2 DSK6713_AIC23_LEFTHPVOL  Left channel headphone volume */  \
0x0179,  /* 3 DSK6713_AIC23_RIGHTHPVOL Right channel headphone volume */ \
0x0011,  /* 4 DSK6713_AIC23_ANAPATH    Analog audio path control */      \
0x0000,  /* 5 DSK6713_AIC23_DIGPATH    Digital audio path control */     \
0x0000,  /* 6 DSK6713_AIC23_POWERDOWN  Power down control */             \
0x0043,  /* 7 DSK6713_AIC23_DIGIF      Digital audio interface format */ \
0x0081,  /* 8 DSK6713_AIC23_SAMPLERATE Sample rate control */            \
0x0001   /* 9 DSK6713_AIC23_DIGACT     Digital interface activation */   \
};


MCBSP_Config AIC23CfgData = {
        MCBSP_FMKS(SPCR, FREE, NO)              |
        MCBSP_FMKS(SPCR, SOFT, NO)              |
        MCBSP_FMKS(SPCR, FRST, YES)             |
        MCBSP_FMKS(SPCR, GRST, YES)             |
        MCBSP_FMKS(SPCR, XINTM, XRDY)           |
        MCBSP_FMKS(SPCR, XSYNCERR, NO)          |
        MCBSP_FMKS(SPCR, XRST, YES)             |
        MCBSP_FMKS(SPCR, DLB, OFF)              |
        MCBSP_FMKS(SPCR, RJUST, RZF)            |
        MCBSP_FMKS(SPCR, CLKSTP, DISABLE)       |
        MCBSP_FMKS(SPCR, DXENA, OFF)            |
        MCBSP_FMKS(SPCR, RINTM, RRDY)           |
        MCBSP_FMKS(SPCR, RSYNCERR, NO)          |
        MCBSP_FMKS(SPCR, RRST, YES),

        MCBSP_FMKS(RCR, RPHASE, SINGLE)         |
        MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |
        MCBSP_FMKS(RCR, RWDLEN2, DEFAULT)       |
        MCBSP_FMKS(RCR, RCOMPAND, MSB)          |
        MCBSP_FMKS(RCR, RFIG, NO)               |
        MCBSP_FMKS(RCR, RDATDLY, 0BIT)          |
        MCBSP_FMKS(RCR, RFRLEN1, OF(0))         | // This changes to 1 FRAME
        MCBSP_FMKS(RCR, RWDLEN1, 32BIT)         | // This changes to 32 bits per frame
        MCBSP_FMKS(RCR, RWDREVRS, DISABLE),

        MCBSP_FMKS(XCR, XPHASE, SINGLE)         |
        MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |
        MCBSP_FMKS(XCR, XWDLEN2, DEFAULT)       |
        MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
        MCBSP_FMKS(XCR, XFIG, NO)               |
        MCBSP_FMKS(XCR, XDATDLY, 0BIT)          |
        MCBSP_FMKS(XCR, XFRLEN1, OF(0))         | // This changes to 1 FRAME
        MCBSP_FMKS(XCR, XWDLEN1, 32BIT)         | // This changes to 32 bits per frame
        MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

        MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |
        MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |
        MCBSP_FMKS(SRGR, CLKSM, DEFAULT)        |
        MCBSP_FMKS(SRGR, FSGM, DEFAULT)         |
        MCBSP_FMKS(SRGR, FPER, DEFAULT)         |
        MCBSP_FMKS(SRGR, FWID, DEFAULT)         |
        MCBSP_FMKS(SRGR, CLKGDV, DEFAULT),

        MCBSP_MCR_DEFAULT,
        MCBSP_RCER_DEFAULT,
        MCBSP_XCER_DEFAULT,

        MCBSP_FMKS(PCR, XIOEN, SP)              |
        MCBSP_FMKS(PCR, RIOEN, SP)              |
        MCBSP_FMKS(PCR, FSXM, EXTERNAL)         |
        MCBSP_FMKS(PCR, FSRM, EXTERNAL)         |
        MCBSP_FMKS(PCR, CLKXM, INPUT)           |
        MCBSP_FMKS(PCR, CLKRM, INPUT)           |
        MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
        MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
        MCBSP_FMKS(PCR, FSXP, ACTIVEHIGH)       |
        MCBSP_FMKS(PCR, FSRP, ACTIVEHIGH)       |
        MCBSP_FMKS(PCR, CLKXP, FALLING)         |
        MCBSP_FMKS(PCR, CLKRP, RISING)
    };


DSK6713_AIC23_CodecHandle hAIC23_handle;

void comm_poll()					//added for communication/init using polling
{
	poll=1;              			//1 if using polling
	DSK6713_init();    				//init DSP and codec
}


void CODEC_start(Uint32 sample_rate, Uint16 inputsource) 
{
	  DSK6713_init();

	 config.regs[4] = inputsource; // select input source
	  
	 hAIC23_handle=DSK6713_AIC23_openCodec(0, &config); //handle(pointer) to codec
	 DSK6713_AIC23_setFreq(hAIC23_handle, sample_rate);  //set sample rate
	 MCBSP_config(DSK6713_AIC23_DATAHANDLE,&AIC23CfgData);//interface 32 bits toAIC23
	 MCBSP_start(DSK6713_AIC23_DATAHANDLE, MCBSP_XMIT_START | MCBSP_RCV_START |
	 	         MCBSP_SRGR_START | MCBSP_SRGR_FRAMESYNC, 220);//start data channel again


}

void CODEC_stop(){
	DSK6713_AIC23_closeCodec(hAIC23_handle);
}

void output_sample(int out_data)    //for out to Left and Right channels
{
	short CHANNEL_data;
	AIC_data.uint=0;                 //clear data structure
	AIC_data.uint=out_data;          //32-bit data -->data structure

//The existing interface defaults to right channel. To default instead to the
//left channel and use output_sample(short), left and right channels are swapped
//In main source program use LEFT 0 and RIGHT 1 (opposite of what is used here)
	CHANNEL_data=AIC_data.channel[RIGHT]; 			//swap left and right channels
	AIC_data.channel[RIGHT]=AIC_data.channel[LEFT];
	AIC_data.channel[LEFT]=CHANNEL_data;
   if (poll) while(!MCBSP_xrdy(DSK6713_AIC23_DATAHANDLE));//if ready to transmit
		MCBSP_write(DSK6713_AIC23_DATAHANDLE,AIC_data.uint);//write/output data
}

void output_left_sample(short out_data)     	  	 //for output from left channel
{
	AIC_data.uint=0;                              //clear data structure
	AIC_data.channel[LEFT]=out_data;   //data from Left channel -->data structure

	if (poll) while(!MCBSP_xrdy(DSK6713_AIC23_DATAHANDLE));//if ready to transmit
		MCBSP_write(DSK6713_AIC23_DATAHANDLE,AIC_data.uint);//output left channel
}

void output_right_sample(short out_data)  		//for output from right channel
{
	AIC_data.uint=0;                             //clear data structure
	AIC_data.channel[RIGHT]=out_data; //data from Right channel -->data structure

	if (poll) while(!MCBSP_xrdy(DSK6713_AIC23_DATAHANDLE));//if ready to transmit
		MCBSP_write(DSK6713_AIC23_DATAHANDLE,AIC_data.uint);//output right channel
}

Uint32 input_sample()                      	  	//for 32-bit input
{
	short CHANNEL_data;

	if (poll) while(!MCBSP_rrdy(DSK6713_AIC23_DATAHANDLE));//if ready to receive
		 AIC_data.uint=MCBSP_read(DSK6713_AIC23_DATAHANDLE);//read data

//Swapping left and right channels (see comments in output_sample())
	CHANNEL_data=AIC_data.channel[RIGHT]; 			//swap left and right channel
	AIC_data.channel[RIGHT]=AIC_data.channel[LEFT];
	AIC_data.channel[LEFT]=CHANNEL_data;

	return(AIC_data.uint);
}

short input_left_sample()                   		//input to left channel
{
	if (poll) while(!MCBSP_rrdy(DSK6713_AIC23_DATAHANDLE));//if ready to receive
	 AIC_data.uint=MCBSP_read(DSK6713_AIC23_DATAHANDLE);//read into left channel
	return(AIC_data.channel[LEFT]);					//return left channel data
}

short input_right_sample()                  		//input to right channel
{
	if (poll) while(!MCBSP_rrdy(DSK6713_AIC23_DATAHANDLE));//if ready to receive
	 AIC_data.uint=MCBSP_read(DSK6713_AIC23_DATAHANDLE);//read into right channel
	return(AIC_data.channel[RIGHT]); 				//return right channel data
}
void comm_intr(Uint32 sample_rate, Uint16 inputsource)	//for communication/init using interrupt
{
	poll=0;                        	//0 since not polling
   IRQ_globalDisable();           	//disable interrupts
   CODEC_start(sample_rate,inputsource);     //c6713_dsk_init() 	init DSP and codec
	CODECEventId=MCBSP_getXmtEventId(DSK6713_AIC23_codecdatahandle);//McBSP1 Xmit

#ifndef using_bios						//do not need to point to vector table
  	IRQ_setVecs(vectors);     			//point to the IRQ vector table
#endif										//since interrupt vector handles this

  	IRQ_map(CODECEventId, 11);			//map McBSP1 Xmit to INT11
  	IRQ_reset(CODECEventId);    		//reset codec INT 11
   IRQ_globalEnable();       			//globally enable interrupts
  	IRQ_nmiEnable();          			//enable NMI interrupt
   IRQ_enable(CODECEventId);			//enable CODEC eventXmit INT11

	output_sample(0);        			//start McBSP interrupt outputting a sample comme une pompe qu'on prime (DG dec 2012)
}
