/*****************************************************************************************
 * Filename : main.c
 * Description : Contains the main function to initialise and run LED, Interrupts,
 * 				 and state machine
 * Author : Maanas Makam Dileep Kumar
 * Reference :
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "led.h"
#include "timer.h"
#include "fsm.h"
#include "tsi.h"
#include "switch.h"
#include "log.h"

#define PWM_PERIOD 255

/* Function Name : main
 * Description : Function to initialize hardware and start the state machine
 * Parameters : None
 * Return Type: None*/
int main (void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
	/* Initialize RGB LED in PWM mode*/
	Init_RGB_LED_PWM(PWM_PERIOD);

	/*Initialize SysTick*/
	Init_SysTick();

	/*Initialize touch slider*/
	Touch_init();

	/*Initialize switch as GPIO interrupt*/
	init_switch();

	/*Reset timer*/
	reset_timer();

	log("Starting main loop of state machine\n\r");
	state_machine();

	return 0;
}
