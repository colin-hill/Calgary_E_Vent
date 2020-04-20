#define SERIAL_DEBUG

#include "FailureMode.h"




void failure_mode(VentilatorState &state) {



#ifdef SERIAL_DEBUG
    Serial.println(F("Failure Mode"));
    Serial.println(F("Error Code:"));
    Serial.println(state.errors);
    Serial.println(F("AC Mode State:"));
    Serial.println(state.ac_state);
    Serial.println(F("VC Mode State:"));
    Serial.println(state.vc_state);
#endif //SERIAL_DEBUG

    // TODO: THIS SHOULD RETURN SOMETHING IF IT HAS THIS TYPE!
    // IT MIGHT NOT BE THIS, THOUGH.
    return;
}
