#include "usart.h"
#include "transceiver.h"
#include <stdlib.h>

uint8_t sendData[8];
uint8_t UART5recieved;
struct usartdata us5;
          
void transceiver_setup(void) {
	for(int i = 0; i<32; i++) { 
		us5.emptybuf[i] = 0;
	}
	us5.currentPackPos = 0;
	us5.packmaxsize = PACKETSIZE;
	
}


uint8_t sendcharUART(uint8_t *ch, uint16_t cnt) {

	HAL_UART_Transmit(&huart4, (uint8_t *)0xFFFF, 2 , 100);
	HAL_UART_Transmit(&huart4, (uint8_t *)"St", 2 , 100);
	HAL_UART_Transmit(&huart4, (uint8_t *)ch, cnt, 100);
	HAL_UART_Transmit(&huart4, (uint8_t *)"En", 2 , 100);
	HAL_UART_Transmit(&huart4, (uint8_t *)0xFFFF, 2 , 100);

	
	return 0;
}



/*
,-----+-----+-----+------+------------+- - - -+-------------,
| SOF | ID  | LEN | TYPE | HEAD_CKSUM | DATA  | DATA_CKSUM  |
| 0-1 | 1-4 | 1-4 | 1-4  | 0-4        | ...   | 0-4         | <- size (bytes)
'-----+-----+-----+------+------------+- - - -+-------------'

SOF ......... start of frame, usually 0x01 (optional, configurable)
ID  ......... the frame ID (MSb is the peer bit)
LEN ......... number of data bytes in the frame
TYPE ........ message type (used to run Type Listeners, pick any values you like)
HEAD_CKSUM .. header checksum

DATA ........ LEN bytes of data
DATA_CKSUM .. data checksum (left out if LEN is 0)
*/



uint8_t recieverUART() {
 
	if (us5.newdata==1) {    // if new data recieved by hal uart5
	us5.newdata=0;
	
	//spos = us5.startPackPos; epos = us5.currentPackPos;
	//memcpy(us5.uartbuf, us5.receivedData, us5.packmaxsize);
	
		
		

	
}
	return 0;
}





	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart==&huart4)
    {
    }
    if(huart==&huart5)
    {
        UART5_RxCpltCallback();
    }
}

    void UART5_RxCpltCallback(void)
{

		HAL_UART_Receive_IT(&huart5,(uint8_t*)us5.receiveBufferUART5HAL,1);			
		
		us5.uartbuf[us5.currentPackPos++]=us5.receiveBufferUART5HAL[0];          	// after interrupt copying symbol to array
		
				if((us5.uartbuf[us5.currentPackPos-2]==83)    && (us5.spfound ==0))
				{
						if(us5.uartbuf[us5.currentPackPos-1]==116)									//  header of packet is "St"
						{
							us5.startPackPos =  us5.currentPackPos;  		// Start of data of the packet is here.  (in us5.uartbuf)
							us5.spfound=1;			//start packet found. In case if uart stream consists 0xFFFF (not as a header)
						}
				}
				
			if ((us5.uartbuf[us5.currentPackPos-2]==69) && (us5.spfound == 1)) 
			{
				if(us5.uartbuf[us5.currentPackPos-1] == 110) 
				{
			    us5.endPackPos = us5.currentPackPos - 2;			// got end of packet    
					us5.uartbuf[0]= us5.uartbuf[us5.currentPackPos];
					us5.newdata = 1;
					us5.spfound = us5.currentPackPos = 0;	
					
					       memset (us5.datapacket, 0, sizeof(us5.datapacket));
							for (int i = 0; i<(us5.endPackPos-us5.startPackPos); i++) 
								{
							us5.datapacket[i] = us5.uartbuf[i+us5.startPackPos]; 
							}
						
					
				}
			
			}
			
			if(us5.currentPackPos==PACKETSIZE)				// end of packet have not achieved.  Throwing away data :(
				{
				us5.spfound = us5.newdata = us5.currentPackPos = 0;	
					memset (us5.uartbuf, 0, sizeof(us5.uartbuf));
				}
		
}
	
            
      
