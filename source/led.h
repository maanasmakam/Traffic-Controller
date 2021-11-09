/*****************************************************************************************
 * Filename : led.h
 * Description : Contains the header for LED functions for PWM of the RGB Led.
 * Author : Maanas Makam Dileep Kumar
 * Reference :
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#ifndef LED_H_
#define LED_H_
#include "MKL25Z4.h"
#include <stdio.h>

/* Function Name : Init_RGB_LED_PWM
 * Description : Function to Enable PWM on RGB LEDS.
 * Parameters : uint16_t period
 * Return Type: None*/
void Init_RGB_LED_PWM(uint16_t);

/* Function Name : led_on
 * Description : Function to set PWM output on LED
 * Parameters : uint32_t color
 * Return Type: None*/
void led_on(uint32_t);

/* Function Name : led_transition
 * Description : Function to compute transition color values of PWM for LED
 * Parameters : uint32_t start_color, uint32_t end_color, int n
 * Return Type: uint32_t*/
uint32_t led_transition(uint32_t start_color,uint32_t end_color, int n);

#endif /* LED_H_ */
