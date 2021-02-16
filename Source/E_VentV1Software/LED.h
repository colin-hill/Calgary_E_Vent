#ifndef LED_H
#define LED_H

#include "MachineStates.h"
#include "PinAssignments.h"


const int LED_OFF = 1;
const int LED_ON = 0;

void setupLEDPins(void);

void control_mode_LEDs(VentilatorState &state);

void control_inhale_exhale_LEDs(VentilatorState &state);

void acmode_LED(VentilatorState &state);

void vcmode_LED(VentilatorState &state);


#endif
