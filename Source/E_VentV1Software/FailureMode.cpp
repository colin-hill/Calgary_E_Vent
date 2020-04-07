#define SERIAL_DEBUG

#include "FailureMode.h"
#include "alarms.h"


void failure_mode(LiquidCrystal &displayName, uint16_t &errors, float peakPressure, float peepPressure, float controllerTemperature) {
#ifdef SERIAL_DEBUG
    Serial.println("Failure Mode");
    Serial.print("PIP: ");
    Serial.println(peakPressure);
    Serial.print("PEEP: ");
    Serial.println(peepPressure);
    Serial.print("Controller Temperature: ");
    Serial.println(controllerTemperature);
    Serial.println("Error Code:");
    Serial.println(errors);
#endif //SERIAL_DEBUG

    //TODO: Set motor velocity to zero

    handle_alarms(displayName, errors, peakPressure, peepPressure, controllerTemperature);
    

    while(1) {
    	//Infinite loop
    	//Requires power cycle of full system to exit
    }	

}