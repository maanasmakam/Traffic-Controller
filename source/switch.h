/*****************************************************************************************
 * Filename : switch.h
 * Description : Contains the header for functions switch as gpio interrupt.
 * Author : Maanas Makam Dileep Kumar
 * Reference :
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#include "stdbool.h"
#ifndef SWITCH_H_
#define SWITCH_H_

/* Function Name : init_switch
 * Description : Function to initialize switch as GPIO interrupt
 * Parameters : None
 * Return Type: None*/
void init_switch();

/* Function Name : get_switch_status
 * Description : Function to check if gpio interrupt occurred.
 * Parameters : None
 * Return Type: bool*/
bool get_switch_status(void);

/* Function Name : reset_switch
 * Description : Reset the switch_on_flag
 * Parameters : None
 * Return Type: None*/
void reset_switch();

/* Function Name : disable_switch
 * Description : Disable the switch GPIO interrupt
 * Parameters : None
 * Return Type: None*/
void disable_switch();

/* Function Name : enable_switch
 * Description : Enable the switch GPIO interrupt
 * Parameters : None
 * Return Type: None*/
void enable_switch();

#endif /* SWITCH_H_ */
