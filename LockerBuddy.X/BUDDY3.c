/*==============================================================================
 File: BUDDY3.c
 Date: January 31, 2023
 
 BUDDY3 (PIC12F1840) hardware initialization function.
==============================================================================*/

#include	"xc.h"				// XC compiler general include file

#include	"stdint.h"			// Integer definition
#include	"stdbool.h"			// Boolean (true/false) definition

#include    "BUDDY3.h"          // Include hardware constant definitions

void init(void)
{
	// Initialize oscillator
	
	OSCCON = 0b01101000;		// PLL off, 4 MHz HF internal oscillator
	
	// Initialize user ports and peripherals:

	OPTION_REG = 0b00101000;	// Weak pull-ups on, falling INT interrupt,
								// TRM0 internal from CPS, 1:1 (no pre-scaler)
	WPUA = 0b00001000;			// Enable weak pull-up on RA3 (S1 input)
	PORTA = 0;					// Clear port input registers and output latches
	LATA = 0;					// before configuring port pins
	ANSELA = 0b00000000;		// Set all I/O pins to digital
	TRISA = 0b00011000;			// Set RA3 and RA4 as inputs, other pins as output
	
	WDTCON = 0b00010100;		// WDT off, div 32768 (~1s period)

	// TODO Enable interrupts if needed
	
}


