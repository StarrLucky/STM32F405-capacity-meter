#include "usart.h"
#include "transceiver.h"

uint8_t sendData[8];
uint8_t receivedData[10];
uint8_t str[]="»USART Transmit\r\n»";




uint8_t sendcharUART(uint8_t *ch) {
	
	HAL_UART_Transmit(&huart4, (uint8_t *)"Cheers\n", 7 , 1000);
	HAL_UART_Transmit(&huart4, (uint8_t *)ch, sizeof(ch) , 1000);
	

	
	return 0;
}
