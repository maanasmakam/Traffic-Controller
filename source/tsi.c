/*****************************************************************************************
 * Filename : tsi.c
 * Description : Includes the functions required to initialize and read values
 * 				 from the Touch Sensor Interface and determine if a touch event occurred.
 * 				 These include Touch_init(),Touch_Detect().
 * Author : Maanas Makam Dileep Kumar
 * Reference : Logic taken from Alexander Dean Git.
 * Link : https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#include <MKL25Z4.H>
#include "stdbool.h"

#define TOUCH_OFFSET 605  /* offset value to be subtracted*/
#define TOUCH_DATA (TSI0->DATA & 0xFFFF)/*macro for extracting the count from data register*/
#define THRESHOLD 20 /*to detect proper touch*/
/* Function Name : Touch_init
 * Description : Function to initialize the Touch Slider Interface
 * Parameters : None
 * Return Type: None*/
void Touch_init()
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK; /* enabling the clock*/
	TSI0->GENCS = TSI_GENCS_MODE(0u) | /*operating in non-noise mode*/
		      TSI_GENCS_REFCHRG(0u) | /*reference oscillator charge and discharge value 500nA*/
		      TSI_GENCS_DVOLT(0u) | /*oscillator voltage rails set to default*/
		      TSI_GENCS_EXTCHRG(0u) | /*electrode oscillator charge and discharge value 500nA*/
		      TSI_GENCS_PS(0u) |  /*frequency clock divided by one*/
	              TSI_GENCS_NSCN(31u) | /*scanning the electrode 32 times*/
	              TSI_GENCS_TSIEN_MASK | /*enabling the TSI module*/
	              TSI_GENCS_EOSF_MASK; /*writing one to clear the end of scan flag*/
}

/* Function Name : Touch_Detect
 * Description : Function to detect a touch on the TSI module. The cutoff value is OFFSET+100 for detection.
 * Parameters : None
 * Return Type: bool*/
bool Touch_Detect(void)
{
	unsigned int val = 0; /* variable to store touch slider value*/
	TSI0->DATA = TSI_DATA_TSICH(10u);
	TSI0->DATA |= TSI_DATA_SWTS_MASK; /*software trigger to start the scan*/
	while (!(TSI0->GENCS & TSI_GENCS_EOSF_MASK )); /* waiting for the scan to complete 32 times*/
	val = TOUCH_DATA;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; /*writing one to clear the end of scan flag*/
	int touch_val=val- TOUCH_OFFSET; /*computation to remove offset from the read value*/
	if(touch_val > THRESHOLD ) /*if a proper touch is detected, return true*/
	{
		return true;
	}
	else
	{
		return false;
	}
}


