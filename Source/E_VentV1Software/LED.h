#ifndef LED_H
#define LED_H

#include "MachineStates.h"
#include "PinAssignments.h"

void setup_LED_pins(void);

void control_mode_LEDs(VentilatorState &state);

void control_inhale_exhale_LEDs(VentilatorState &state);

void acmode_LED(VentilatorState &state);

void vcmode_LED(VentilatorState &state);


#endif
