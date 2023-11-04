/*!
\file   delay_1ms_irq.c
\date   2020-11-13
\author Fulvio Vivas <fyvivas@unicauca.edu.co>
\brief  Generating a delay of 1 ms in PIC18F4550 using Timer1.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2020. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/

#include <xc.h>
#include <pic16f887.h>  /*Header file PIC18f4550 definitions*/
#include "fuses.h"		/* Configuration header file */
#include "Timer0_1ms.h"

//#define _XTAL_FREQ 4000000//frecuencia del oscilador
//TMR1=65535-(delay)(1Mhz)/8 -1

#define Pulse PORTB  		/* Define Pulse as LATB to output on PORTB */


/*********************Interrupt Service Routine for Timer1************************/
void __interrupt() Timer1_ISR()
{
    TMR1=0xFC16;
    Pulse = ~Pulse;    /* Toggle Value at PortB to generate waveform of 500 Hz */   
    PIR1bits.TMR1IF=0; /* Make Timer1 Overflow Flag to '0' */
}

void Timer1_start()
{
    GIE=1;              /* Enable Global Interrupt */
    PEIE=1;             /* Enable Peripheral Interrupt */
    TMR1IE=1;           /* Enable Timer1 Overflow Interrupt */
    TMR1IF=0;
    T1CON=0x00;         /* Enable 16-bit TMR1 Register,No pre-scale,use internal clock,Timer OFF */
    TMR1=0xFC16;        /* Load Count for generating delay of 1ms */
    TMR1ON=1;           /* Turn-On Timer1 */
}
