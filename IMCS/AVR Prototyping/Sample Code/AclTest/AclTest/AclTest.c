/*
 * AclTest.c
 *
 * Created: 9/16/2011 5:15:52 PM
 *  Author: J
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define SS		PB0
#define MOSI	PB2
#define MISO	PB3
#define SCK		PB1

#define ACLX_EN
#define ACT_INACT_CTRL 0x27

#define SEND_REQ(isRd,addr,data) ( (uint16_t)(isRd << 16) | (uint16_t)(addr << 14) | (uint8_t)(data) )
#define RD_REG(reg) SEND_REQ(1,reg,0)
#define WR_REG(reg,data) SEND_REQ(0,reg,data)

#define 

ISR(SPI_STC_vect)
{
	cli();
	
	int spi_data = (int)SPDR;
	
	PORTE = spi_data;
	
	sei();
}

int main(void)
{
    while(1)
    {
		// Disable Interrupts
		cli();
		
		// Enable LED Operation
		DDRE = 0x04;
		
        // Setup SPI DDR
		DDRB = _BV(MOSI) | _BV(SCK);
		
		// Enable Interrupts, SPI, Master - clock rate 500 KHz
		SPCR = _BV(SPIE) | _BV(SPE) | _BV(MSTR) | _BV(SPR0);
		
		// Enable Interrupts
		sei();
		
		// Ask for A Read
    }
}