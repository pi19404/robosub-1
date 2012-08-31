/*
 * LedTest.c
 *
 * Created: 9/7/2011 8:49:04 PM
 *  Author: J
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum ledMode
{
	INCREMENT,
	DECREMENT
} LedMode;

volatile LedMode current_direction = INCREMENT;

///
/// ISR routine changes active LED on 0.125 second intervals.
/// -LED Cycle is LED0 -> ... -> LED3 -> ... -> LED0, and repeats indefinitely.
///
ISR(TIMER1_COMPA_vect)
{
	cli();
	
	// Check Direction Change
	if( _BV(PE7) == PORTE )
	{
		// Shift to previous LED
		current_direction = DECREMENT;
	}
	else if ( _BV(PE4) == PORTE )
	{
		// Shift to next LED
		current_direction = INCREMENT;
	}
	
	// Increment LED
	if( current_direction == INCREMENT )
	{
		PORTE <<= 1;
	}
	// Decrement LED
	else 
	{
		PORTE >>= 1;
	}
	
	// Reset the Timer
	TCNT1 = 0;
	
	sei();
}

int main(void)
{
	// Set LEDS on Port E
	DDRE |= 0xF0;
	PORTE |= 0x10;
	
	// Set Timer 1024 pre-scaled, interrupt every 0.125 seconds.
	cli();
	TCCR1B |= _BV(CS12) | _BV(CS10);
	OCR1A = 1024;
	TCNT1 = 0;
	TIMSK |= _BV(OCIE1A);
	sei();
	
    while(1)
    {
    }
}