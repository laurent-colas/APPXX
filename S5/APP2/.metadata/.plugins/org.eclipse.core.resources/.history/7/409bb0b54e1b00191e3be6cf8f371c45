/****************************************************************************/
/*  C6701.cmd                                                               */
/*  Copyright (c) 2012 Texas Instruments Incorporated                       */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on a C6701               */
/*                 device.  Use it as a guideline.  You will want to        */
/*                 change the memory layout to match your specific C6xxx    */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

MEMORY
{
    VECS:        o = 0x00000000  l = 0x00000200   /* Interrupt vector table */
    IRAM:        o = 0x00000200  l = 0x0000FFE0   /* 64kB Internal RAM */
    SDRAM:       o = 0x80000000  l = 0x01000000   /* 16MB External SDRAM */
}                                              
                                               
SECTIONS                                       
{                                              
    .intvecs       >  VECS  /* Custom section for interrupt vector table */
    .text          >  IRAM                    
    .stack         >  IRAM                    
    .bss           >  IRAM                    
    .cio           >  IRAM                    
    .const         >  IRAM                    
    .data          >  IRAM                    
    .switch        >  IRAM                    
    .sysmem        >  IRAM                    
    .far           >  IRAM                    
    .args          >  IRAM                    
    .ppinfo        >  IRAM
    .ppdata        >  IRAM
  
    /* COFF sections */
    .pinit         >  IRAM
    .cinit         >  IRAM
  
    /* EABI sections */
    .binit         >  IRAM
    .init_array    >  IRAM
    .neardata      >  IRAM
    .fardata       >  IRAM
    .rodata        >  IRAM
    .c6xabi.exidx  >  IRAM
    .c6xabi.extab  >  IRAM
}
