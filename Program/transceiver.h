#ifndef _TRAN_H
#define _TRAN_H

#define PACKETSIZE 30      //expected of known size of the packet which consists data + headers

uint8_t sendcharUART(uint8_t *ch, uint16_t cnt);
void ftoa(float f, char *str);
void transceiver_setup(void);
uint8_t recieverUART(void);
void UART5_RxCpltCallback(void);

struct usartdata {

	volatile uint8_t	receiveBufferUART5HAL[2];
	volatile uint8_t receivedData[32];
	volatile uint8_t datapacket[PACKETSIZE];
	volatile  uint8_t emptybuf[32], uartbuf[32];
	
	
volatile uint8_t newdata,  spfound; 

volatile uint8_t bytesToReceive;
volatile uint8_t currentPackPos;
volatile uint8_t endPackPos;
volatile uint8_t	startPackPos;
uint8_t packmaxsize;  

uint8_t packagedone;
} ;

extern struct usartdata us5;

















#endif

