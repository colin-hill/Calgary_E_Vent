#include "alarms.h"
#include "pressure.h"
#include "elapsedMillis.h"


// ----------------------------------------------------------------------
// Timers for alarms
// ----------------------------------------------------------------------

elapsedMillis alarmBuzzerTimer;
elapsedMillis highPressureAlarmTimer;
elapsedMillis lowPressureAlarmTimer;
elapsedMillis highPEEPAlarmTimer;
elapsedMillis lowPEEPAlarmTimer;
elapsedMillis disconnectAlarmTimer;
elapsedMillis highTempAlarmTimer;
elapsedMillis apneaAlarmTimer;
elapsedMillis deviceFaiulureAlarmTimer;


// ----------------------------------------------------------------------
// Function definitions
// ----------------------------------------------------------------------

uint16_t check_high_pressure(const float pressure) {
    if (pressure > MAX_PRESSURE) {
        return HIGH_PRESSURE_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_low_pressure(const float pressure) {
    if (pressure < MIN_PRESSURE) {
        return LOW_PRESSURE_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_pressure(const float pressure) {
    return check_high_pressure(pressure) | check_low_pressure(pressure);
}


uint16_t check_high_peep(const float pressure) {
    if (pressure > MAX_PEEP_PRESSURE) {
        return HIGH_PEEP_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_low_peep(const float pressure) {
    if (pressure < MIN_PEEP_PRESSURE) {
        return LOW_PEEP_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_peep(const float pressure) {
    return check_high_peep(pressure) | check_low_peep(pressure);
}


void handle_alarms(uint16_t &errors) {
    if(errors){ //There is an unserviced error
        //Control the buzzer
        if(alarmBuzzerTimer > ALARM_SOUND_LENGTH*1000){
            alarmBuzzerTimer = 0; //Reset the timer
            digitalWrite(ALARM_BUZZER_PIN,!digitalRead(ALARM_BUZZER_PIN)); //Toggle the buzzer output pin
        }

        //Provide the appropriate screen for the error, error flags held in a 16 bit unsigned integer
        if(errors & HIGH_PRESSURE_ALARM){
            //Display high pressure alarm screen********
        }
        else if(errors & LOW_PRESSURE_ALARM){
            //Display low pressure alarm screen********
        }
        else if(errors & HIGH_PEEP_ALARM){
            //Display high PEEP alarm screen********
        }
        else if(errors & LOW_PEEP_ALARM){
            //Display low PEEP alarm screen********
        }
        else if(errors & DISCONNECT_ALARM){
            //Display disconnect alarm (also a low pressure alarm)
        }
        else if(errors & HIGH_TEMP_ALARM){
            //Display high temp alarm screen********
        }
        else if(errors & APNEA_ALARM){
            //Display the apnea alarm screen********
        }
        else if(errors & DEVICE_FAILURE_ALARM){
            //Display the device failure alarm********
        }
        else{
            errors = 0;
        }
    }
    else{
        alarmBuzzerTimer = 0;
        digitalWrite(ALARM_BUZZER_PIN,LOW);
    }
}
