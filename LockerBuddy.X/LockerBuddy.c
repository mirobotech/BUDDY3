/*==============================================================================
 Project: LockerBuddy.c                 Link: mirobo.tech/buddy3
 Date: February 2, 2023

 Locker Buddy locker/closet/drawer alarm.
  
 Use Locker-Buddy to monitor your locker for an intrusion since the last time
 you opened it. Locker Buddy wakes up after you open your locker, lights its
 eyes and greets you with a chirp. Press and hold the button to tell Locker
 Buddy it's you, and to reset its door counter.
  
 When you close the locker door, Locker Buddy waits for a few seconds before
 turning off its eyes and going to sleep. This means it won't accidentally 
 count it as an intrusion if you change your mind and re-open the door within
 its sleep delay.
  
 If someone else has opened the door and not reset it, locker buddy will wink
 its eyes with an alarm sound and then beep for the number of times the door
 has been opened since that last time it was reset (hopefully only by you!).
  
 LED3 shines infrared (IR) light towards the locker door, and the reflected
 light from the door is sensed by phototransistor Q1. These have a limited
 range. If Locker Buddy doesn't go to sleep, try moving it closer to the door,
 or stick something light or reflective, such as a piece of masking tape or
 white paper, to the door directly in front of where LED3 and Q1 will be.
  
 Locker Buddy spends a lot of its time asleep to save battery power. It uses
 the microcontroller's watch dog time (WDT) to wake it approximately once per
 second to check the door state. With the LEDs off and the microcontroller
 asleep, current draw is minimized.
 =============================================================================*/


#include    "xc.h"              // XC compiler general include file

#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include	"BUDDY3.h"          // Include hardware constants and functions

bool asleep;                    // Sleep/wake state (asleep = true)

unsigned char doorOpened;       // Number of times the door has opened
unsigned char wakeCounter;      // Counter used to transition sleep/wake states
unsigned char i;                // Generic loop/index counter

// Time delay constants to transition between sleep and wake modes. Must be > 0.
#define sleepDelay 15           // Sleep delay in seconds after door closes
#define awakeDelay 2            // Wake delay in seconds after door opens

// Blinks the eyes on an off
void blink(void)
{
		RIGHTEYE = 0;           // Eyes off...
		LEFTEYE = 0;
		__delay_ms(150);
		RIGHTEYE = 1;           // Eyes on
		LEFTEYE = 1;
}

// Changes the brightness of each eye LED using PWM (Pulse-width Modulation).
// The brightness changes from the start value to the end value (these must be
// between 0 and 100).
void setEyes(unsigned char pwmStart, unsigned char pwmEnd)
{
    unsigned char p;
	while(pwmStart != pwmEnd)
	{
		if(pwmStart < pwmEnd)   // Adjust PWM parameter up or down
			pwmStart++;
		else
            pwmStart--;

		RIGHTEYE = 0;           // Turn LEDs off at the start of each PWM cycle
		LEFTEYE = 0;

		for(p = 100; p != 0; p --)
		{
			if(pwmStart == p)   // Turn LEDs on when pwmStart == p(wm) count
            {
				RIGHTEYE = 1;
                LEFTEYE = 1;
            }
		}
	}
}

// Lights the IR LED and check for response from the phototransistor. Returns
// 0 (false) if a reflective target is seen (door is closed) or 1 (true) if
// no target is there (door is open).
bool doorOpen(void)
{
    IRLED = 1;                  // Turn IR LED on
    __delay_us(100);            // Delay for phototransistor response
    if(Q1 == 0)                 // If reflection is seen, door is closed
    {
        IRLED = 0;              // Turn LED off to save batteries
        return(false);          // Return 0
    }
    else
    {
        IRLED = 0;              // Turn LED off to save batteries
        return(true);           // Return 1
    }
}

// Make short beep sound of the specified period for the specified # of cycles.
// Period and cycles are character variables and must be between 0 - 255.
void beep(unsigned char period, unsigned char cycles)
{
	unsigned char i;
    while(cycles != 0)          // Make cycles number of sound waves
	{
		BEEPER = 1;             // Beeper on for the high part of sound wave
		for(i = period; i != 0; i --);
		BEEPER = 0;             // Beeper off for the low part of sound wave
		for(i = period; i != 0; i --);
		cycles --;
	}
}

// Main Locker-Buddy program starts here.
int main(void)
{
	init();                     // Initialize oscillator, I/O, and peripherals
	
    RIGHTEYE = 1;               // Power-on eye test
    LEFTEYE = 1;
	beep(40,100);				// Power-on beeper test
	RIGHTEYE = 0;
    LEFTEYE = 0;
    
    // Set starting conditions
	doorOpened = 0;             // Clear door opened count
    wakeCounter = 0;            // Reset wake counter
    asleep = true;              // Start in sleep mode
	SWDTEN = 1;                 // Enable WDT (Watch Dog Timer)
    
	while(1)                    // This main loop repeats forever
	{
		while(asleep)
        {
            SLEEP();                // Shut down until next WDT wake-up
            
            if(doorOpen())          // Check door state after waking up
                wakeCounter ++;     // Count door open cycles
            else
                wakeCounter = 0;    // Reset count if door is closed
            
            if(wakeCounter == awakeDelay)   // Time to wake up?
            {
                if(doorOpened < 255) // If door openings < character var. limit
                    doorOpened ++;   // count door openings and wake up!

                setEyes(1,100);     // Wake up, brighten eyes using PWM

                RIGHTEYE = 1;       // Keep eyes on now
                LEFTEYE = 1;
                SLEEP();            // Wait until next WDT cycle
                
                RIGHTEYE = 0;       // Blink and...
                LEFTEYE = 0;
                beep(80,80);        // Say 'hello'
                RIGHTEYE = 1;
                LEFTEYE = 1;
                beep(60,60);
                beep(72,80); 
                asleep = false;     // Switch to awake mode
            }
        }
        
        while(!asleep)              // We're awake!
        {
            if(doorOpened > 1)      // Check if the door has been open before
            {
                SLEEP();            // Nap until next WDT cycle
                for(i = 3; i != 0; i --)    // Uh oh, this is not the first one!
                {
                    RIGHTEYE = 0;				
                    beep(80,60);    // Wink and make alert sounds
                    RIGHTEYE = 1;
                    LEFTEYE = 0;
                    beep(120,50);
                    LEFTEYE = 1;
                }
                for(i = doorOpened-1; i != 0; i--)   // Announce # of door events
                {                                                   
                    SLEEP();
                    RIGHTEYE = 0;
                    LEFTEYE = 0;
                    beep(160,80);   // Blink and boop for every door event
                    RIGHTEYE = 1;
                    LEFTEYE = 1;
                }
            }
            wakeCounter = 0;        // Reset wake counter
            
            while(wakeCounter < sleepDelay)
            {
                SLEEP();            // Nap until next WDT wake-up
                blink();
                
                if(S1 == 0)         // Is pushbutton pressed?
                {
                    doorOpened = 0; // If yes, reset doorCount
                    RIGHTEYE = 0;   // Blink
                    LEFTEYE = 0;
                    beep(60,60);    // Say 'ok'
                    RIGHTEYE = 1;
                    LEFTEYE = 1;
                    beep(72,80);
                    blink();        // Blink, blink
                }
                
                if(doorOpen())      // Door still open?
                    wakeCounter = 0;// Reset count
                else
                    wakeCounter ++; // Door Closed? Delay before sleeping
            }
            
            SLEEP();                // Do one more WDT sleep cycle
            setEyes(100,0);         // Getting sleepy... dim the eyes
            RIGHTEYE = 0;           // Keep eyes off
            LEFTEYE = 0;
            wakeCounter = 0;        // Reset wake counter
            asleep = true;          // Sleep
		}
    }
}
