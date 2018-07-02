#include "gpio.h"
#include "meter.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "string.h"

 

float MAX_CAP_CAPACITY_T;
uint32_t MAX_CAP_CAPACITY_T_inTicks;

volatile uint32_t *DWT_CONTROL = (uint32_t *)0xE0001000;
volatile uint32_t *DWT_CYCCNT = (uint32_t *)0xE0001004; 
volatile uint32_t *DEMCR = (uint32_t *)0xE000EDFC; 
uint32_t startTime, elapsedTime, cnt2;
float XXX;
float op_time;
struct cap CAP;
void meter_setup(void) {
	MAX_CAP_CAPACITY_T = MAX_CAP_CAPACITY * RESISTOR_VALUE; 
	MAX_CAP_CAPACITY_T_inTicks =  (tim_clock-1)*MAX_CAP_CAPACITY_T ;
	
	

	
	

	
}


/* 
Set discharge pin to INPUT (so it can't discharge the capacitor)
Record the start time with millis()
Set charge pin to OUTPUT and make it HIGH
Check the voltage repeatedly in a loop until it gets to 63.2% of total voltage.
After the cap is charged, subtract the current time from the start time to find out how long the capacitor took to charge.
Divide the Time in seconds by the charging Resistance in ohms to find the Capacitance.
Report the value with serial.print
Discharge the capacitor. To do this:
Set the charge pin to Input
Set the discharge pin to OUTPUT and make it LOW
Read the voltage to make sure the capacitor is fully discharged
Loop and do it again
*/





	
	
float GetCapacity2(void) {

		HAL_ADC_Start_DMA(&hadc3, (uint32_t*)CAP.adcdata, 10);
			      
	//	if (CheckConnection() == 0) { HAL_ADC_Stop_DMA(&hadc3);
//		return 999; }
	
	
		CAP.adcdata[10] = 0; CAP.adcdata[1] = 0; CAP.adcdata[2] = 0; CAP.adcdata[3] = 0; CAP.adcdata[4] = 0; CAP.adcdata[5] = 0; CAP.adcdata[6] = 0; CAP.adcdata[7] = 0;
		CAP.adcdata[8] = 0; CAP.adcdata[9] = 0; CAP.adcdata[10] = 0;
	

	
	
	
	
		*DEMCR = *DEMCR | 0x01000000; 						 // enable the use DWT
		*DWT_CYCCNT = 0; 												// Reset cycle counter
		*DWT_CONTROL = *DWT_CONTROL | 1 ;			// enable cycle counter
	
		CAP_CHARGER(CHARGE);
		   
		
		while(CAP.adcdata[0]<CAPCHARGED)		// waiting for cap to be charged          // sdelat trigger that would be set inside dma interrupt when  adc val =CAPCHARGED
		{	                                                                           // or use opamp. when U is = CAPCHARGED,  it till trigger ext. interrupt pin which woul mean that cap is charged.
		}
		
		elapsedTime=*DWT_CYCCNT-startTime;	
		op_time= (elapsedTime)/tim_clock;// count/F_CPU
	  CAP.capacity = ((op_time)/RESISTOR_VALUE) *1000000.0;  			// uF  
	 __enable_irq(); 
		CAP_CHARGER(DISCHARGE);
		HAL_ADC_Stop_DMA(&hadc3);
		

		return CAP.capacity;

}		
	
	
	
	

void CAP_CHARGER(int state) 
	{
	
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	
	if (state == CHARGE) {

	//Making dischargepin INPUT, LOW 
	GPIO_InitStruct.Pin = discharge_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(discharge_GPIO_Port, &GPIO_InitStruct);


		
	//Making chargepin OUTPUT, HIGH 
	GPIO_InitStruct.Pin = charge_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(charge_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(charge_GPIO_Port, charge_Pin, GPIO_PIN_SET);
	 
	__disable_irq(); 	
	startTime= *DWT_CYCCNT;
		}
	
	else if (state == DISCHARGE) {
		
		GPIO_InitTypeDef GPIO_InitStruct;	
		//Making chargepin INPUT 
		GPIO_InitStruct.Pin = charge_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(charge_GPIO_Port, &GPIO_InitStruct);

		//Making dischargepin OUTPUT, LOW 
		GPIO_InitStruct.Pin = discharge_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(discharge_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(discharge_GPIO_Port, discharge_Pin, GPIO_PIN_RESET); 
		
		
	
		int i = 0;
		if ((MAX_CAP_CAPACITY_T*1000)<1) { HAL_Delay(1000); } 
		else if ((MAX_CAP_CAPACITY_T*1000)>1) 
			{
		
		while(CAP.adcdata[1] >20)   // waiting for cap to discharge close to ~0 v.
			{
			HAL_Delay(200);
			i = i+1;
			if (i*200 >MAX_CAP_CAPACITY_T*1000) { break; }		//  Carefull if R*C <1
			}
			}
	}
	}


int CheckConnection(void) 
	{
		// charging cap, disconnecting io pins and then measuring voltage by ADC. if there is nothing, then cap is not connected. 
		// probably ADC will measure data from the air so this method not very applicable. 
		CAP.adcdata[0] = 0;
//		HAL_ADC_Start(&hadc1);
		CAP_CHARGER(CHARGE);
		HAL_Delay(500);
		
		//Making chargepin INPUT 
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = charge_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(charge_GPIO_Port, &GPIO_InitStruct);
		// discharge pin is INPUT also. So there is only  cap connected to adc pin and ground.
					
		if (CAP.adcdata[0] > ((CAPCHARGED/100) * 30)) 
		{ 
		CAP_CHARGER(DISCHARGE);	
		return 1;	
		}
		else
			{ 
		CAP_CHARGER(DISCHARGE);
		return 0;
			}

	}



float GetCapacity(void) 
	{
	/*
	cnt2 = 0; CAP.adcdata[0] = 0; CAP.timerdata[0] =0; CAP.chargetime_ticks =0; CAP.chargetime = 0; CAP.capacity = 0;	
	HAL_TIM_Base_Start(&htim5);	
		
		
		*DEMCR = *DEMCR | 0x01000000; 						 // enable the use DWT
		*DWT_CYCCNT = 0; 												// Reset cycle counter
		*DWT_CONTROL = *DWT_CONTROL | 1 ;			// enable cycle counter
		
		CAP_CHARGER(CHARGE);

	
			
		while(1)		// waiting for cap to be charged 
		{	
			
			CAP.timerdata[0] = __HAL_TIM_GetCounter(&htim5);	
			CAP.adcdata[0] = HAL_ADC_GetValue(&hadc3);
			cnt2++;
			// if time of charging more than MAX_CAP_CAPACITY_T, ms, then cap is more than MAX_CAP_CAPACITY mF, or there is no more cap betwen terminals at all.
		if ((CAP.adcdata[0]>CAPCHARGED) || (CAP.timerdata[0] > MAX_CAP_CAPACITY_T_inTicks)) { break; }		
		}
		
	
		CAP.chargetime_ticks = __HAL_TIM_GetCounter(&htim5);	
		elapsedTime=*DWT_CYCCNT-startTime;	
		CAP.adcdata[0] = HAL_ADC_GetValue(&hadc3);
		CAP.timerdata[0] = __HAL_TIM_GetCounter(&htim5);	
		
		
		
		
		
	
	 
	op_time= (elapsedTime+(21*cnt2))/tim_clock;// count/F_CPU
	 
	CAP.capacity = ((op_time)/RESISTOR_VALUE) *1000000;  			// uF
	 
	XXX = (((CAP.chargetime_ticks)/(tim_clock)*1000000.0)/RESISTOR_VALUE);		// uF

		
		
		
		CAP_CHARGER(DISCHARGE);
		HAL_TIM_Base_Stop(&htim5);	
	 
	 
	 return CAP.capacity;
	*/
	return 0;

	}
	
			