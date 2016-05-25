/*
 * main.c
 *
 * Created: 2016-03-02 4:15:28 PM
 * Author : take-iwiw
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "myCommon.h"
#include <util/delay.h>

#include "./myLibraries/myStdio.h"

/*** Internal Const Values ***/

/*** Internal Static Variables ***/

/*** Internal Function Declarations ***/


/* for ADNS9800 */
#include "./spi/spi.h"
#define ADNS_SS_DDR  DDRB
#define ADNS_SS_PORT PORTB
#define ADNS_SS_BIT  2

void sendData(uint8_t addr, uint8_t data)
{
	CLR_BIT(ADNS_SS_PORT, ADNS_SS_BIT);
	spiSendBlocking(addr | 0x80);
	spiSendBlocking(data);
	SET_BIT(ADNS_SS_PORT, ADNS_SS_BIT);
}

uint8_t readData(uint8_t addr)
{
	CLR_BIT(ADNS_SS_PORT, ADNS_SS_BIT);
	spiSendBlocking(addr & 0x7f);
	_delay_us(100);
	uint8_t data = spiRecv();
	SET_BIT(ADNS_SS_PORT, ADNS_SS_BIT);
	return data;
}
/* ! for ADNS9800 */


/*** External Function Defines ***/
int main(void)
{
	_delay_ms(100);

	sei();	//SET_BIT(SREG, SREG_I);
	stdioInit();

	print("Hello");
	print("Start");

	/* for ADNS9800 */
	SPI_OPEN_PRM prm;
	prm.order = SPI_OPEN_ORDER_MSB_FIRST;
	prm.role = SPI_OPEN_ROLE_MASTER;
	prm.mode = SPI_OPEN_MODE_3;
	prm.div = SPI_OPEN_DIV_64;
	prm.speed = SPI_OPEN_SPEED_X1;
	prm.blocking = SPI_OPEN_BLOCKING_YES;
	spiOpen(&prm);

	SET_BIT(ADNS_SS_DDR, ADNS_SS_BIT);	// CS = OUTPUT

	// read product ID and revision ID to communication test
	printDec(readData(0x00));
	_delay_ms(10);
	printDec(readData(0x01));
	_delay_ms(10);
	printDec(readData(0x3f));

	getchar();
	sendData(0x3f, 0x5a);		// software reset, just in case

	while(1) {
		printDec(readData(0x02)); print(" ");
		printDec(readData(0x03)); print(" ");
		printDec(readData(0x04)); print(" ");
		printDec(readData(0x05)); print(" ");
		printDec(readData(0x06)); print("\r\n");
		_delay_ms(10);
	}
	/* ! for ADNS9800 */

}


