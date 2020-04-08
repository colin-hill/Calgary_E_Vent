#define SERIAL_DEBUG

#include "FailureMode.h"


void failure_mode(uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.println("Failure Mode");
    Serial.println("Error Code:");
    Serial.println(errors);
#endif //SERIAL_DEBUG

    //TODO: Set motor velocity to zero
    

    while(1) {
    	//Infinite loop
    	//Requires power cycle of full system to exit
    }	

}