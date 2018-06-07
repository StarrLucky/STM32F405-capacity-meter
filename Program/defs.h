#ifndef ALL_H
#define _ALL_H

#define ADC_0V_VALUE                            0
#define ADC_1V_VALUE                            1241
#define ADC_2V_VALUE                            2482
#define ADC_3V_VALUE                            3723
#define ADC_ARRAY_SIZE 												  100



struct diag {
	uint64_t adcpackets;
	uint32_t usbpackets;
	uint32_t usbpackets_sent;
	uint32_t usbpackets_fail;
	
} ;
extern struct diag stats;
extern struct diag *statsp;  //pointer to sruct diag

extern int  
LEDS, LED0, LED1;					  



#endif
