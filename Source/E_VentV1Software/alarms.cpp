#include "alarms.h"
#include "pressure.h"
#include "elapsedMillis.h"
#include "LCD.h"
#include "breathing.h"




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


VentilatorState handle_alarms(VentilatorState state, LiquidCrystal &displayName) {
    if (state.errors) { // There is an unserviced error
        // Control the buzzer
        if (alarmBuzzerTimer > (ALARM_SOUND_LENGTH*1000)) {
            alarmBuzzerTimer = 0; //Reset the timer
            digitalWrite(ALARM_BUZZER_PIN,!digitalRead(ALARM_BUZZER_PIN)); //Toggle the buzzer output pin
            digitalWrite(ALARM_LED_PIN,!digitalRead(ALARM_LED_PIN));
            digitalWrite(ALARM_RELAY_PIN,!digitalRead(ALARM_RELAY_PIN));

        }

        // Provide the appropriate screen for the error, error flags held in a 16 bit unsigned integer
        if (state.errors & HIGH_PRESSURE_ALARM) {
            // Display high pressure alarm screen
            displayHighPressureAlarm(displayName, state.peak_pressure, LCD_MAX_STRING);
        }
        else if (state.errors & LOW_PRESSURE_ALARM) {
            // Display low pressure alarm screen
            displayLowPressureAlarm(displayName, state.peak_pressure, LCD_MAX_STRING);
        }
        else if (state.errors & HIGH_PEEP_ALARM) {
            // Display high PEEP alarm screen
            displayHighPEEPAlarm(displayName, state.peep_pressure, LCD_MAX_STRING);
        }
        else if (state.errors & LOW_PEEP_ALARM) {
            // Display low PEEP alarm screen
            displayLowPEEPAlarm(displayName, state.peep_pressure, LCD_MAX_STRING);
        }
        else if (state.errors & DISCONNECT_ALARM) {
            // Display disconnect alarm (also a low pressure alarm)
            displayDisconnectAlarm(displayName);
        }
        else if (state.errors & HIGH_TEMP_ALARM) {
            // Display high temp alarm screen
            displayTemperatureAlarm(displayName, state.controller_temperature, LCD_MAX_STRING);
        }
        else if (state.errors & APNEA_ALARM) {
            // Display the apnea alarm screen
            displayApneaAlarm(displayName);
        }
        else if (state.errors & DEVICE_FAILURE_ALARM) {
            displayDeviceFailureAlarm(displayName);
            state.machine_state = FailureMode;
        }
        else{
            // TODO: I (Calvin) am actually pretty nervous about this default.
            // I feel like we should display an unspecified error or something.
            assert(false);  // This should NOT happen.
            state.errors = 0;
        }
    }
    else {
        alarmBuzzerTimer = 0;
        digitalWrite(ALARM_BUZZER_PIN,LOW);
        digitalWrite(ALARM_LED_PIN,LOW);
        digitalWrite(ALARM_RELAY_PIN,LOW);
    }

    return state;
}
