/*****************************************************************************************
 * Filename : tsi.h
 * Description : Includes the declarations for the functions required to initialize and read values
 * 				 from the Touch Sensor Interface and determine if a touch event occurred.
 * Author : Maanas Makam Dileep Kumar
 * Reference :
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#include "stdbool.h"
#ifndef TSI_H_
#define TSI_H_

/* Function Name : Touch_init
 * Description : Function to initialize the Touch Slider Interface
 * Parameters : None
 * Return Type: None*/
void Touch_init();

/* Function Name : Touch_read
 * Description : Function to read value from the Touch Slider Interface
 * Parameters : None
 * Return Type: bool*/
bool Touch_Detect();

#endif /* TSI_H_ */
