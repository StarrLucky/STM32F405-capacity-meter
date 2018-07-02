#ifndef _METER_H
#define _METER_H
//SETTINGS 
#define tim_clock														 72000000.0 									 // CPU clock
//#define analogPin                        		 ADC1    				  	  // analog pin for measuring capacitor voltage
#define chargePin                            charge_Pin				   // (Reference in main.h)  pin to charge the capacitor - connected to one end of the charging resistor
#define dischargePin                         discharge_Pin			//	(Reference in main.h)  pin to discharge the capacitor
#define RESISTOR_VALUE                       10000.0					 	 // change this to resistor value you are using
#define ADC_RESOLUTION											
#define CAPCHARGED													 648    				//   ADC value of charged cap voltage. (ADCmax divided by 63.2% of the 3.3v).   If adc resolution is 12bit, then  CAPCHARGED  = 4096 / 2.08 =1969		= 2589,
																														//  6 bit -- 40,  10 bit -- 648, 8 bit -- 162. 
																														// Best results achieved with this settings of ADC: 8bit, prescaler div4; or  ADC 10 bit, prescaler div2
#define MAX_CAP_CAPACITY										 0.001					//   F                                               
																													/* (MAX_CAP_CAPACITY_T = MAX_CAP_CAPACITY* RESISTOR_VALUE). Time to charge capacitor, if it's have capacity MAX_CAP_CAPACITY.
                                                           if R = 10K, then Time to charge cap with 1000uf is 1 second. 
																													But at the same time, if tested capacitor will have capacity = 1pF, then charge time will be 1 * 10^(-8) second!
																													Which means, that for clocking there is need to use 32bit timer without prescaler.  (UPD: using cycle counter now). 
																											(if clock is 24 MHZ then timer5 ticks will be every 4.16666667e-8 seconds). So then it could work, if not, then 
																											there is way to reduce abilities to measure caps from like 10 pF, to 100 uF, for example, and/or using higher R. 
																											Better solution is to use different resistors: measuring cap for the 1st time, and if it's charging very long time, 
																										then use lower resistance, to more precociousness. 
																									And if it's charges for like just very small amount of time, so, then, probably it's pF,  then use higher resistance. */
extern uint32_t MAX_CAP_CAPACITY_T_inTicks; 		// MAX_CAP_CAPACITY_T relative to TIM5 clock ticks. I cant compare time in seconds vs tim5 ticks inside loop, while sizing capacity.
extern float MAX_CAP_CAPACITY_T;
//
#define _CAP																1									
#define _IND																2
#define CHARGE															1
#define DISCHARGE														0


void meter_setup(void);
float GetCapacity(void);
float GetCapacity2(void);
int CheckConnection(void);
void CAP_CHARGER(int state);

struct cap {
volatile  uint32_t adcdata[20];
volatile 	uint32_t timerdata[4];
volatile 	uint32_t chargetime_ticks;
volatile	float capacity;
	float capacityARR[5];
	uint32_t chargetime;
	
} ;


#endif







