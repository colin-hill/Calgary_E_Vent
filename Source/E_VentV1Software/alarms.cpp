#include "alarms.h"
#include "pressure.h"
#include "elapsedMillis.h"
#include "LCD.h"
#include "breathing.h"
#include "Motor.h"

#include <assert.h>




// ----------------------------------------------------------------------
// Timers for alarms
// ----------------------------------------------------------------------

elapsedMillis alarmBuzzerTimer;


// ----------------------------------------------------------------------
// Function definitions
// ----------------------------------------------------------------------

uint16_t check_high_pressure(const float pressure, UserParameter *userParameters) {
    SelectedParameter selectedParameter = e_HighPIPAlarm;
	if (pressure > userParameters[selectedParameter].value) {
        return HIGH_PRESSURE_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_low_pressure(const float pressure, UserParameter *userParameters) {
	SelectedParameter selectedParameter = e_LowPIPAlarm;
  Serial.print("Low PEEP Setting: ");
    Serial.println(userParameters[selectedParameter].value);
    if (pressure < userParameters[selectedParameter].value) {
        return LOW_PRESSURE_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_pressure(const float pressure, UserParameter *userParameters) {
    return check_high_pressure(pressure, userParameters) | check_low_pressure(pressure, userParameters);
}


uint16_t check_high_peep(const float pressure, UserParameter *userParameters) {
    SelectedParameter selectedParameter = e_HighPEEPAlarm;
    if (pressure > userParameters[selectedParameter].value) {
        return HIGH_PEEP_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_low_peep(const float pressure, UserParameter *userParameters) {
    SelectedParameter selectedParameter = e_LowPEEPAlarm;
    //Serial.print("Low PEEP Setting: ");
    //Serial.println(userParameters[selectedParameter].value);
    if (pressure < userParameters[selectedParameter].value) {
        return LOW_PEEP_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_peep(const float pressure, UserParameter *userParameters) {
    return check_high_peep(pressure, userParameters) | check_low_peep(pressure, userParameters);
}

/*uint16_t check_controller_temperature(const uint16_t temperature){
    if (temperature > MAX_CONTROLLER_TEMPERATURE) {
        return HIGH_TEMP_ALARM;
    } else {
        return 0;
    }
}*/

uint16_t check_motor_position(const long int current_position, const long int expected_position) {
    if (current_position > expected_position + POSITION_TOLERANCE) {
        Serial.println("Out of tolerance");
        return MECHANICAL_FAILURE_ALARM;
    }
    else if (current_position < expected_position - POSITION_TOLERANCE) {
        Serial.println("Out of tolerance");
        return MECHANICAL_FAILURE_ALARM;
    } else {
        return 0;
    }
}

uint16_t check_respiratory_rate(VentilatorState &state, UserParameter *userParameters){

  float max_respiratory_rate = userParameters[(int)e_HighRespiratoryRateAlarm].value;

  if(state.calculated_respiratory_rate > max_respiratory_rate){

    return HIGH_RR_ALARM;
  }
  
  return 0;
}

void alarm_debounce_reset(VentilatorState &state) {

  state.alarm_outputs = state.this_breath_errors | state.last_breath_errors;
  state.last_breath_errors = state.this_breath_errors;
  state.this_breath_errors = 0;

  return;

}

void loop_alarm_manager(elapsedMillis &externalDisplayTimer, elapsedMillis &alarmSilenceTimer,
                        volatile boolean &alarmReset, LiquidCrystal &alarmDisplay,
                        LiquidCrystal &parameterDisplay, HardwareSerial &externalDisplay,
                        VentilatorState &state, UserParameter *userParameters,
                        SelectedParameter &currentlySelectedParameter) {

  //If there is a mechanical failure, decide what happens next
  if (state.errors & MECHANICAL_FAILURE_ALARM){
    //Increment count
    state.mechanical_failure_count += 1;
    if (state.mechanical_failure_count >= 2){
      state.machine_state = FailureMode;
      state.errors |= FULL_DEVICE_FAILURE;
    }
    else {
    //Go to calibration
      state.machine_state = MotorZeroing;
      state.zeroing_state = CommandHome;
    }
  } 

  state.this_breath_errors |= state.errors;

  state.errors = 0;

  state.alarm_outputs |= state.this_breath_errors;




  if (state.alarm_outputs != 0) {
    control_alarm_output(alarmSilenceTimer, alarmReset, state);
    digitalWrite(ALARM_LED_PIN, HIGH);
  }
  else {
    digitalWrite(ALARM_BUZZER_PIN, LOW);
    digitalWrite(ALARM_LED_PIN, LOW);
  }


  control_alarm_displays(alarmDisplay, parameterDisplay, state, userParameters, currentlySelectedParameter);


  if (externalDisplayTimer > (EXTERNAL_DISPLAY_DWELL*S_TO_MS)) {
    state.external_display = !state.external_display;
    externalDisplayTimer = 0;
  }

  control_external_display(externalDisplayTimer, externalDisplay, state);

  return;   
}

void control_alarm_output(elapsedMillis &alarmSilenceTimer, volatile boolean &alarmReset, VentilatorState &state) {

  Serial.print(F("alarmReset value: "));
  Serial.println(alarmReset);

  cli();
  if (true == alarmReset) {
    alarmReset = false;
    alarmSilenceTimer = 0;
    state.silenced_alarms = state.alarm_outputs;
  }
  sei();


  
  Serial.print(F("Silence Timer: "));
  Serial.println(alarmSilenceTimer);

  if ((alarmSilenceTimer > (ALARM_SILENCE_TIME*S_TO_MS)) || (state.alarm_outputs &(~state.silenced_alarms))) {

     

    if (alarmSilenceTimer > (ALARM_SILENCE_TIME*S_TO_MS)) {
       state.silenced_alarms = 0;
    }

    digitalWrite(ALARM_BUZZER_PIN, HIGH);
    /*if (alarmBuzzerTimer > (ALARM_SOUND_LENGTH*S_TO_MS)) {
            // Reset the timer
            alarmBuzzerTimer = 0;

            // Toggle the buzzer output pins
            digitalWrite(ALARM_BUZZER_PIN,!digitalRead(ALARM_BUZZER_PIN));
            //digitalWrite(ALARM_LED_PIN,!digitalRead(ALARM_LED_PIN));
            digitalWrite(ALARM_RELAY_PIN,!digitalRead(ALARM_RELAY_PIN));
    }
  }*/
  }
  else {
    digitalWrite(ALARM_BUZZER_PIN, LOW);
  }
  
  return;
}

void control_alarm_displays(LiquidCrystal &alarmDisplay, LiquidCrystal &parameterDisplay, VentilatorState &state, UserParameter *userParameters, SelectedParameter &currentlySelectedParameter) {
    
    SelectedParameter currentParameter = e_HighPIPAlarm;
    float maxPIP = userParameters[(int)currentParameter].value;
    float tempMaxPIP = userParameters[(int)currentParameter].tmpValue;
  
    currentParameter = e_LowPIPAlarm;
    float minPIP = userParameters[(int)currentParameter].value;
    float tempMinPIP = userParameters[(int)currentParameter].tmpValue;
  
    currentParameter = e_HighPEEPAlarm;
    float maxPEEP = userParameters[(int)currentParameter].value;
    float tempMaxPEEP = userParameters[(int)currentParameter].tmpValue;
  
    currentParameter = e_LowPEEPAlarm;
    float minPEEP = userParameters[(int)currentParameter].value;
    float tempMinPEEP = userParameters[(int)currentParameter].tmpValue;
  
    currentParameter = e_LowPlateauPressureAlarm;
    float lowPlateauPressure = userParameters[(int)currentParameter].value;
    float tempLowPlateauPressure = userParameters[(int)currentParameter].tmpValue;

    currentParameter = e_HighRespiratoryRateAlarm;
    float respiratoryRateAlarm = userParameters[(int)currentParameter].value;

    if(((int)currentlySelectedParameter >= e_HighPIPAlarm) && (e_None != (int)currentlySelectedParameter)){ //If there is a alarm parameter selected display it preferentially

      displayAlarmParameters(currentlySelectedParameter, alarmDisplay, userParameters);
    }
    else if (state.alarm_outputs & FULL_DEVICE_FAILURE) {
      displayDeviceFailureAlarm(alarmDisplay);
    }
    else if (state.alarm_outputs & MECHANICAL_FAILURE_ALARM) {
      displayMechanicalFailureAlarm(alarmDisplay);
    }
    else {
      displayMultipleAlarms(alarmDisplay, maxPIP, minPIP, maxPEEP, minPEEP, respiratoryRateAlarm, state);
    }

    return;
}

void control_external_display(elapsedMillis &externalDisplayTimer, HardwareSerial &externalDisplay, VentilatorState &state) {

  if (state.alarm_outputs & FULL_DEVICE_FAILURE) {
    char outputString[] = "FAIL";
    externalDisplay.print('v');
    externalDisplay.print(outputString);
  }
  else if (state.alarm_outputs & MECHANICAL_FAILURE_ALARM) {
    char outputString[] = "CAL";
    externalDisplay.print('v');
    externalDisplay.print(outputString);
  }
  else if (state.alarm_outputs & HIGH_PRESSURE_ALARM) {
    char outputString[] = "HpIp";
    externalDisplay.print('v');
    externalDisplay.print(outputString);
  }
  else if (state.alarm_outputs & LOW_PEEP_ALARM) {
    char outputString[] = "LEEp";
    externalDisplay.print('v');
    externalDisplay.print(outputString);
  }
  else if (state.alarm_outputs & HIGH_PEEP_ALARM){
    char outputString[] = "HEEp";
    externalDisplay.print('v');
    externalDisplay.print(outputString);
  }
  else if (state.alarm_outputs & LOW_PRESSURE_ALARM) {
    char outputString[] = "LpIp";
    externalDisplay.print('v');
    externalDisplay.print(outputString);
  }
  else if (state.alarm_outputs & HIGH_RR_ALARM) {
    char outputString[] = "HIrr";
    externalDisplay.print('v');
    externalDisplay.print(outputString);
  }
  else {

    if (false == state.external_display) {
      int extDispPIP = (int) round(state.peak_pressure);
      char outputString[] = "Ip";
      externalDisplay.print('v');
      externalDisplay.print(outputString);
      externalDisplay.print(extDispPIP);
      
    }
    else {
      int extDispPEEP = (int) round(state.peep_pressure);
      char outputString[] = "Ep";
      externalDisplay.print('v');
      externalDisplay.print(outputString);
      externalDisplay.print(extDispPEEP);
    }
  }
}





void setUpAlarmPins()
{
  pinMode(ALARM_SWITCH_PIN,INPUT_PULLUP);

  pinMode(ALARM_LED_PIN,OUTPUT);
  pinMode(ALARM_BUZZER_PIN,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ALARM_SWITCH_PIN),alarmResetISR,FALLING);

  return;
}
