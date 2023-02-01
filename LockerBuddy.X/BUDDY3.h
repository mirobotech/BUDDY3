/*==============================================================================
 File: BUDDY3.h
 Date: January 31, 2023
 
 BUDDY3 (PIC12F1840) symbolic constant definitions and function prototypes.
==============================================================================*/

// I/O Port devices

#define LED1        RA0         // Right 'eye' LED output
#define RIGHTLED    RA0         // Right 'eye' LED output
#define LED2        RA1         // Left 'eye' LED output
#define LEFTLED     RA1         // Left 'eye' LED output
#define LED3        RA2         // IR LED output
#define IRLED       RA2         // IR LED output
#define S1			RA3			// Switch S1 input
#define Q1          RA4         // IR phototransistor input
#define IR          RA4         // IR phototransistor input
#define P1			RA5			// Piezo beeper output
#define BEEPER		RA5			// Piezo beeper output

// Clock frequency for delay macros and simulation

#define _XTAL_FREQ	4000000     // Set clock frequency for time delay calculations
#define FCY	_XTAL_FREQ/4        // Processor instruction cycle time

// TODO - Add function prototypes for all functions in BUDDY3.c here:

void init(void);                // Initialization function prototype