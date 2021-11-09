/*****************************************************************************************
 * Filename : timer.c
 * Description : Includes the functions required for handling the state machine using the
 * 				 Systick Timer. The functions include Init_SysTick, SysTick_Handler, now,
 * 				 get_timer, and reset_timer functions.
 * Author : Maanas Makam Dileep Kumar
 * Reference : Logic taken from Alexander Dean book chapter 7
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#include <stdio.h>
#include "MKL25Z4.h"
#include "timer.h"

static uint32_t time=0; /*counter to keep track of the time elapsed since the last restart*/
static uint32_t start_time=0; /* counter to keep track of the time elapsed since the start*/
extern volatile int checktsi; /* flag variable to poll tsi after 62.5ms*/

#define TICK_TIME_IN_MS 62.5 /*value in ms of one tick count*/
#define CLOCK_DIV 256 /*counter divider for 1/16s */

/* Function Name : Init_SysTick
 * Description : Function to initialize the SysTick Timer
 * Parameters : None
 * Return Type: None*/
void Init_SysTick(void)
{
	SysTick->LOAD = (48000000L / CLOCK_DIV); /*Load systick counter value*/
	NVIC_SetPriority (SysTick_IRQn, 3); /*Set Systick interrupt priority*/
	SysTick -> VAL = 0; /*Set VAL to 0*/
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk ; /*enable systick*/
}

/* Function Name : SysTick_Handler
 * Description : Interrupt Handler for Systick Interrupt
 * Parameters : None
 * Return Type: None*/
void SysTick_Handler(void)
{
	start_time++;
	time++;
	checktsi = 1;
}

/* Function Name : now
 * Description : Returns the time elapsed in mssince start of execution
 * Parameters : None
 * Return Type: uint32_t*/
uint32_t now()
{
	return (start_time * TICK_TIME_IN_MS);
}

/* Function Name : reset_timer
 * Description : Resets the time variable to zero.
 * Parameters : None
 * Return Type: None*/
void reset_timer()
{
	time=0;
}

/* Function Name : get_timer
 * Description : Returns the time in ticks since the last reset function.
 * Parameters : None
 * Return Type: uint32_t*/
uint32_t get_timer()
{
	return time;
}
