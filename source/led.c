/*****************************************************************************************
 * Filename : led.c
 * Description : Contains the LED related functions to enable PWM output.
 * 				 The functions included are Init_RGB_LED_PWM,led_on, and led_transition.
 * Author : Maanas Makam Dileep Kumar
 * Reference : Logic for PWM from textbook by Alexander Dean, Chapter 7
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#include "MKL25Z4.h"
#include <stdio.h>

/*defining macros for RED LED i.e. PORTB Pin18*/
#define RED_LED_PIN 18
#define RED_LED_PORT PORTB
#define RED_LED_GPIO GPIOB
#define RED_LED_SCG5_MASK SIM_SCG5_PORTB_MASK
#define RED_LED_PIN_CTRL_REG PORTB->PCR[RED_LED_PIN]

/*defining macros for GREEN LED i.e. PORTB Pin19*/
#define GREEN_LED_PIN 19
#define GREEN_LED_PORT PORTB
#define GREEN_LED_GPIO GPIOB
#define GREEN_LED_SCG5_MASK SIM_SCG5_PORTB_MASK
#define GREEN_LED_PIN_CTRL_REG PORTB->PCR[GREEN_LED_PIN]

/*defining macros for BLUE LED. i.e. PORTD Pin1*/
#define BLUE_LED_PIN 1
#define BLUE_LED_PORT PORTD
#define BLUE_LED_GPIO GPIOD
#define BLUE_LED_SCG5_MASK SIM_SCG5_PORTD_MASK
#define BLUE_LED_PIN_CTRL_REG PORTD->PCR[BLUE_LED_PIN]

typedef uint32_t color_t;

/*extracting R,G,B values from 24 bit hexadecimal*/
#define RED_VAL_MASK(color) ((color >> 16) & 0xFF)
#define GREEN_VAL_MASK(color) ((color >> 8) & 0xFF)
#define BLUE_VAL_MASK(color) (color & 0xFF)

/*1/16 of 1s is 0.0625s*/
#define SCALE 0.0625

/* Function Name : Init_RGB_LED_PWM
 * Description : Function to Enable PWM on RGB LEDS.
 * Parameters : uint16_t period
 * Return Type: None*/
void Init_RGB_LED_PWM(uint16_t period)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	/*Setting up port multiplexing to GPIO*/
	RED_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
	RED_LED_PIN_CTRL_REG |= PORT_PCR_MUX(3);

	/*Setting up port multiplexing to GPIO*/
	GREEN_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
	GREEN_LED_PIN_CTRL_REG |= PORT_PCR_MUX(3);

	/*Setting up port multiplexing to GPIO*/
	BLUE_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
	BLUE_LED_PIN_CTRL_REG |= PORT_PCR_MUX(4);

	// Configure TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	//set clock source for tpm: 48 MHz
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	//load the counter and mod
	TPM2->MOD = period-1;
	TPM0->MOD = period-1;
	//set TPM count direction to up with a divide by 2 prescaler
	TPM2->SC =  TPM_SC_PS(1);
	TPM0->SC =  TPM_SC_PS(1);

	// Continue operation in debug mode
	TPM2->CONF |= TPM_CONF_DBGMODE(3);
	TPM0->CONF |= TPM_CONF_DBGMODE(3);

	// Set channels to edge-aligned low-true PWM
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	// Set initial duty cycle
	TPM2->CONTROLS[0].CnV = 0;
	TPM2->CONTROLS[1].CnV = 0;
	TPM0->CONTROLS[1].CnV = 0;

	// Start TPM
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM2->SC |= TPM_SC_CMOD(1);
}

/* Function Name : led_on
 * Description : Function to set PWM output on LED
 * Parameters : uint32_t color
 * Return Type: None*/
void led_on(color_t color)
{
	TPM2->CONTROLS[0].CnV = RED_VAL_MASK(color);
	TPM2->CONTROLS[1].CnV = GREEN_VAL_MASK(color);
	TPM0->CONTROLS[1].CnV = BLUE_VAL_MASK(color);
}

/* Function Name : led_transition
 * Description : Function to compute transition color values of PWM for LED
 * Parameters : uint32_t start_color, uint32_t end_color, int n
 * Return Type: uint32_t*/
color_t led_transition(color_t start_color,color_t end_color, int n)
{
	int sr = RED_VAL_MASK(start_color); /* masking initial red value*/
	int er = RED_VAL_MASK(end_color); /* masking final red value*/
	int sg = GREEN_VAL_MASK(start_color); /* masking initial green value*/
	int eg = GREEN_VAL_MASK(end_color); /* masking final green value*/
	int sb = BLUE_VAL_MASK(start_color); /* masking initial blue value*/
	int eb = BLUE_VAL_MASK(end_color); /* masking final blue value*/
	int mr = ((er-sr) * n * SCALE) + sr; /* computing the transition red value*/
	int mg = ((eg-sg) * n * SCALE) + sg; /* computing the transition green value*/
	int mb = ((eb-sb) * n * SCALE) + sb; /* computing the transition blue value*/
	color_t newcolor = (mr << 16) + (mg << 8) + mb; /* computing the transition rgb value*/
	return newcolor;
}
