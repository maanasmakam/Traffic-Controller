/*****************************************************************************************
 * Filename : switch.c
 * Description : Contains the functions to configure and use the switch on PORTD 3
 * 				 as a GPIO interrupt.
 * Author : Maanas Makam Dileep Kumar
 * Reference : Logic taken from Howdy's PPT.
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#include "MKL25Z4.h"
#include "stdbool.h"
#include "stdio.h"
#include "timer.h"
#include "log.h"

/*macros for configuring PORTD pin3 as GPIO interrupt*/
#define SWITCH_GPIO_PORT GPIOD
#define SWITCH_PIN 3
#define SWITCH_PIN_CTRL_REG PORTD->PCR[SWITCH_PIN]
#define SWITCH_SCGC5_MASK SIM_SCGC5_PORTD_MASK
#define SWITCH_ISFR PORTD->ISFR
#define SWITCH_IRQ PORTD_IRQn

static int switch_on_flag=0;

/* Function Name : init_switch
 * Description : Function to initialize switch as GPIO interrupt
 * Parameters : None
 * Return Type: None*/
void init_switch()
{
	/*Setting switch GPIO*/
	SWITCH_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
	SWITCH_PIN_CTRL_REG |= PORT_PCR_MUX(1);

	/*Enabling Pull up resistor*/
	SWITCH_PIN_CTRL_REG |= PORT_PCR_PE(1) | PORT_PCR_PS(1);

	/*Setting GPIO as input*/
	SWITCH_GPIO_PORT->PDDR &= ~(1 << SWITCH_PIN);

	/*Enabling the GPIO Pin as external interrupt for low level trigger*/
	SWITCH_PIN_CTRL_REG |= PORT_PCR_IRQC(8);

	/*Setting NVIC priority for GPIO interrupt*/
	NVIC_SetPriority(SWITCH_IRQ, 4);

	/*Clearing Pending IRQ requests*/
	NVIC_ClearPendingIRQ(SWITCH_IRQ);

	/*Enabling PORTD GPIO Interrupts*/
	NVIC_EnableIRQ(SWITCH_IRQ);
}

/* Function Name : PORTD_IRQHandler
 * Description : Overwriting default handler.
 * Parameters : None
 * Return Type: None*/
void PORTD_IRQHandler(void)
{
	 if((SWITCH_ISFR & (1 << SWITCH_PIN)) == 0 ) /*verifying cause of interrupt*/
			 return;
	 switch_on_flag=1;
	 SWITCH_ISFR &= (1 << SWITCH_PIN); /*clearing the interrupt flag*/
}

/* Function Name : get_switch_status
 * Description : Function to check if gpio interrupt occurred.
 * Parameters : None
 * Return Type: bool*/
bool get_switch_status(void)
{
	int status = switch_on_flag;
	switch_on_flag=0;
	return status;
}

/* Function Name : reset_switch
 * Description : Reset the switch_on_flag
 * Parameters : None
 * Return Type: None*/
void reset_switch()
{
	switch_on_flag=0;
}

/* Function Name : disable_switch
 * Description : Disable the switch GPIO interrupt
 * Parameters : None
 * Return Type: None*/
void disable_switch()
{
	NVIC_DisableIRQ(SWITCH_IRQ);
}

/* Function Name : enable_switch
 * Description : Enable the switch GPIO interrupt
 * Parameters : None
 * Return Type: None*/
void enable_switch()
{
	NVIC_EnableIRQ(SWITCH_IRQ);
}
