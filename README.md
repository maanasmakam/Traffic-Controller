# Traffic-Controller
Code developed for FRDM KL25Z.

The objective of the project is to simulate a traffic controller system using PWM for LED, TSI module for crosswalk button, and Systick Timer for accurate timing. A GPIO interrupt is used to trigger a crosswalk signal. 

The outline of the state machine designed is shown below :
![fsm](https://user-images.githubusercontent.com/89494511/136880690-aa0b93d4-ed41-4a11-85e1-03821532ce8c.png)

Outline of the states:

stop -> Default starting state. Signals the stop color indicating the vehicles to stop.

go -> Signals the go color indicating the vehicles to go.

warning -> Signals the warning color indicating the vehicles to slow down and get ready to stop.

transit_stop -> Transition state from warning to stop.

transit_warn -> Transition state from go to warning.

transit_go -> Transition state used from stop to go as well as crosswalk to go.

transit_crosswalk -> Transition state from all of the above states to crosswalk upon pressing of button.

crosswalk -> Signals the pedastrians that crosswalk is enabled by continously blinking the crosswalk color.


This repository contains the project files out of which the main source files are:

main.c -> Contains the required hardware initialization and state machine call for execution of the program.

led.c -> Contains all the functions required for the RGB Led to function using PWM.

tsi.c -> Includes the functions required to initialize and return if touch event occurred from the Touch Sensor Interface.

timer.c -> Contains all the functions required to initialize and run the Systick Timer Interrupt.

switch.c -> Contains all the functions required to configure and use the switch on PortD Pin3 as a GPIO Interrupt.

fsm.h -> Contains the switch statement based implementation of the State Machine Design.

tsi.h -> Header file for touch sensor functions.

switch.h -> Header file for switch GPIO functions.

led.h -> Header file for Led PWM functions.

timer.h -> Header file for Systick Interrupt functions.

log.h -> File to enable printf in debug mode only.

fsm.h -> Header file for state machine function.

This project has been executed on the MCUXpresso IDE on the FRDM-KL25Z board.

Both Debug and Release builds have been included in this project.
