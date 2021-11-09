/*****************************************************************************************
 * Filename : fsm.c
 * Description : Contains the state machine function using a switch statement implementation.
 * Author : Maanas Makam Dileep Kumar
 * Reference :
 * Link :
 * Tools Used : MCUXpresso IDE
 *****************************************************************************************/
#include <stdio.h>
#include "MKL25Z4.h"
#include "led.h"
#include "timer.h"
#include "tsi.h"
#include "stdbool.h"
#include "switch.h"
#include "stdbool.h"
#include "log.h"

/*Various colors as defined for the states in the assignment detail*/
#define GO_COLOR  0x229622
#define STOP_COLOR 0x611E3C
#define WARNING_COLOR 0xFFB200
#define CROSSWALK_COLOR 0x001030
#define CROSSWALK_OFF 0x000000

#ifdef DEBUG
#	define GO_TIME 80 /*To track 5s*/
#	define STOP_TIME 80	/*To track 5s*/
#	define WARN_TIME 48	/*To track 3s*/
#else
#	define GO_TIME 320	/*To track 20s*/
#	define STOP_TIME 320 /*To track 20s*/
#	define WARN_TIME 80	/*To track 5s*/
#endif

typedef uint32_t color_t;

#define TRANSIT_TIME 16 /*To track 1s*/
#define CROSSWALK_TIME 160	/*To track 10s*/
#define CROSSWALK_ON_CYCLES 12 /*To track 750ms*/

#define CROSSWALK_MASK 0xF /*To find position in transition*/

color_t start_color; /*variable to track start color during transitions*/
color_t t_color; /*variable to track intermediate colors during transitions*/

volatile int checktsi = 0;	/*flag to check if 62.5ms elapsed*/
volatile int switch_int = 0; /*flag to store if gpio interrupt occurred*/

enum states{go, stop, warning, crosswalk, transit_go, transit_stop, transit_warn,transit_crosswalk};
enum states state=stop;
static bool val; /*flag to store touch detection*/

/* Function Name : state_machine
 * Description : Function to run the state machine
 * Parameters : None
 * Return Type: None*/
void state_machine()
{
	while(1){
		switch_int=get_switch_status(); /*check if interrupt is triggered on the switch*/
		if(checktsi == 1)
		{

			val=Touch_Detect(); /*check if touch is detected*/
			if(((val == true) || (switch_int == 1)) && (state != crosswalk) && (state != transit_crosswalk))
			{
				log("Button press detected %d ms.\n\r",now());
			}
			checktsi = 0; /*reset flag*/
		}

		switch(state)
		{
			case stop:
					if(get_timer() == STOP_TIME){ /*check if time for state transition*/
						log("Transition from stop to transit_go state at %d ms.\n\r",now());
						state=transit_go;
						start_color=STOP_COLOR;
						reset_timer();
					}
					led_on(STOP_COLOR); /*turn on LED in stop color*/
					if(val || switch_int){ /*check if crosswalk is triggered*/
						log("Transition from stop to transit_crosswalk state at %d ms.\n\r",now());
						state=transit_crosswalk;
						switch_int=0;
						start_color=STOP_COLOR;
						reset_timer();
					}
					break;
			case go:
					if(get_timer() == GO_TIME){ /*check if time for state transition*/
						log("Transition from go to transit_warn state at %d ms.\n\r",now());
						state=transit_warn;
						start_color=GO_COLOR;
						reset_timer();
					}
					led_on(GO_COLOR); /*turn on LED in go color*/
					if(val || switch_int){  /*check if crosswalk is triggered*/
						log("Transition from go to transit_crosswalk state at %d ms.\n\r",now());
						state=transit_crosswalk;
						switch_int=0;
						start_color=GO_COLOR;
						reset_timer();
					}
					break;
			case warning:
					if(get_timer() == WARN_TIME){ /*check if time for state transition*/
						log("Transition from warning to transit_stop state at %d ms.\n\r",now());
						state=transit_stop;
						start_color=WARNING_COLOR;
						reset_timer();
					}
					led_on(WARNING_COLOR); /*turn on LED in warning color*/
					if(val || switch_int){  /*check if crosswalk is triggered*/
						log("Transition from warning to transit_crosswalk state at %d ms.\n\r",now());
						state=transit_crosswalk;
						switch_int=0;
						start_color=WARNING_COLOR;
						reset_timer();
					}
					break;
			case transit_go:
					if(get_timer() == TRANSIT_TIME){ /*check if time for state transition*/
						log("Transition from transit_go to go state at %d ms.\n\r",now());
						state=go;
						reset_timer();
					}
					t_color=led_transition(start_color,GO_COLOR,get_timer());
					led_on(t_color); /*turn on LED in transition colors*/
					if(val || switch_int){  /*check if crosswalk is triggered*/
						log("Transition from transit_go to transit_crosswalk state at %d ms.\n\r",now());
						state=transit_crosswalk;
						switch_int=0;
						start_color=t_color;
						reset_timer();
					}
					break;
			case transit_stop:
					if(get_timer() == TRANSIT_TIME){ /*check if time for state transition*/
						log("Transition from transit_stop to stop state at %d ms.\n\r",now());
						state=stop;
						reset_timer();
					}
					t_color=led_transition(start_color,STOP_COLOR,get_timer());
					led_on(t_color); /*turn on LED in transition colors*/
					if(val || switch_int){  /*check if crosswalk is triggered*/
						log("Transition from transit_stop to transit_crosswalk state at %d ms.\n\r",now());
						state=transit_crosswalk;
						switch_int=0;
						start_color=t_color;
						reset_timer();
					}
					break;
			case transit_warn:
					if(get_timer() == TRANSIT_TIME){ /*check if time for state transition*/
						log("Transition from transit_warn to warning state at %d ms.\n\r",now());
						state=warning;
						reset_timer();
					}
					t_color=led_transition(start_color,WARNING_COLOR,get_timer());
					led_on(t_color); /*turn on LED in transition colors*/
					if(val || switch_int){  /*check if crosswalk is triggered*/
						log("Transition from transit_warn to transit_crosswalk state at %d ms.\n\r",now());
						state=transit_crosswalk;
						switch_int=0;
						start_color=t_color;
						reset_timer();
					}
					break;
			case transit_crosswalk:
					disable_switch();
					if(get_timer() == TRANSIT_TIME){ /*check if time for state transition*/
						log("Transition from transit_crosswalk to crosswalk state at %d ms.\n\r",now());
						state=crosswalk;
						reset_timer();
					}
					t_color=led_transition(start_color,CROSSWALK_COLOR,get_timer());
					led_on(t_color); /*turn on LED in transition colors*/
					break;
			case crosswalk:
					if(get_timer() == CROSSWALK_TIME){ /*check if time for state transition*/
						log("Transition from crosswalk to transit_go state at %d ms.\n\r",now());
						state=transit_go;
						start_color=CROSSWALK_COLOR;
						reset_timer();
						enable_switch();
					}
					/*blink LED for 750ms on and 250ms off*/
					if((get_timer() & CROSSWALK_MASK) < CROSSWALK_ON_CYCLES){ /*extracting mod 16 value of get_timer*/
						led_on(CROSSWALK_COLOR);
					}
					else{
						led_on(CROSSWALK_OFF);
					}
					reset_switch();
					break;
		}
	}
}
