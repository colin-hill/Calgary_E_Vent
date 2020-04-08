#include "alarms.h"
#include "pressure.h"
#include "elapsedMillis.h"
#include "LCD.h"




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


machineStates handle_alarms(machineStates machineState,
                            LiquidCrystal &displayName, 
                            uint16_t &errors, float peakPressure, 
                            float peepPressure, float controllerTemperature) {
    
     machineStates next_state = machineState;

    if(errors){ //There is an unserviced error

       

        //Control the buzzer
        if(alarmBuzzerTimer > ALARM_SOUND_LENGTH*1000){
            alarmBuzzerTimer = 0; //Reset the timer
            digitalWrite(ALARM_BUZZER_PIN,!digitalRead(ALARM_BUZZER_PIN)); //Toggle the buzzer output pin
            digitalWrite(ALARM_LED_PIN,!digitalRead(ALARM_LED_PIN));
            digitalWrite(ALARM_RELAY_PIN,!digitalRead(ALARM_RELAY_PIN));

        }

        //Provide the appropriate screen for the error, error flags held in a 16 bit unsigned integer
        if(errors & HIGH_PRESSURE_ALARM){
            //Display high pressure alarm screen
             displayHighPressureAlarm(displayName, peakPressure, LCD_MAX_STRING);
        }
        else if(errors & LOW_PRESSURE_ALARM){
            //Display low pressure alarm screen
            displayLowPressureAlarm(displayName, peakPressure, LCD_MAX_STRING);
        }
        else if(errors & HIGH_PEEP_ALARM){
            //Display high PEEP alarm screen
            displayHighPEEPAlarm(displayName, peepPressure, LCD_MAX_STRING);
        }
        else if(errors & LOW_PEEP_ALARM){
            //Display low PEEP alarm screen
            displayLowPEEPAlarm(displayName, peepPressure, LCD_MAX_STRING);
        }
        else if(errors & DISCONNECT_ALARM){
            //Display disconnect alarm (also a low pressure alarm)
            displayDisconnectAlarm(displayName);
        }
        else if(errors & HIGH_TEMP_ALARM){
            //Display high temp alarm screen
            displayTemperatureAlarm(displayName, controllerTemperature, LCD_MAX_STRING);
        }
        else if(errors & APNEA_ALARM){
            //Display the apnea alarm screen
            displayApneaAlarm(displayName);
        }
        else if(errors & DEVICE_FAILURE_ALARM){
            //Display the device failure alarm
            displayDeviceFailureAlarm(displayName);
            next_state = FailureMode;

        }
        else{
            errors = 0;
        }
    }
    else{
        alarmBuzzerTimer = 0;
        digitalWrite(ALARM_BUZZER_PIN,LOW);
        digitalWrite(ALARM_LED_PIN,LOW);
        digitalWrite(ALARM_RELAY_PIN,LOW);
    }

    return next_state;
}
