#define SERIAL_DEBUG

#include "FailureMode.h"




VentilatorState failure_mode(VentilatorState state) {

    //TODO: Set motor velocity to zero

    //TODO: Set errors to critcal failure


#ifdef SERIAL_DEBUG
    Serial.println("Failure Mode");
    Serial.println("Error Code:");
    Serial.println(state.errors);
    Serial.println("AC Mode State:");
    Serial.println(state.ac_state);
    Serial.println("VC Mode State:");
    Serial.println(state.vc_state);
#endif //SERIAL_DEBUG

    // TODO: THIS SHOULD RETURN SOMETHING IF IT HAS THIS TYPE!
    // IT MIGHT NOT BE THIS, THOUGH.
    return state;
}
