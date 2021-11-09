/*****************************************************************************************
 * Filename : timer.h
 * Description : Includes header for Systick timer functions.
 * Author : Maanas Makam Dileep Kumar
 * Reference :
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/* Function Name : Init_SysTick
 * Description : Function to initialize the SysTick Timer
 * Parameters : None
 * Return Type: None*/
void Init_SysTick(void);

/* Function Name : now
 * Description : Returns the time elapsed since start of execution
 * Parameters : None
 * Return Type: uint32_t*/
uint32_t now();

/* Function Name : reset_timer
 * Description : Resets the time variable to zero.
 * Parameters : None
 * Return Type: None*/
void reset_timer();

/* Function Name : get_timer
 * Description : Returns the time elapsed since the last reset function.
 * Parameters : None
 * Return Type: uint32_t*/
uint32_t get_timer();

#endif /* TIMER_H_ */
