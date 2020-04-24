#ifndef PinAssignments_h
#define PinAssignments_h


//All constants should be written like VARIABLE_NAME

//TODO: assign actual pins

//LCD Pins
const int ALARM_LCD_ENABLE      = 49;
const int ALARM_LCD_RS          = 53;
const int ALARM_LCD_DB4         = 47;
const int ALARM_LCD_DB5         = 45;
const int ALARM_LCD_DB6         = 43;
const int ALARM_LCD_DB7         = 41;

const int VENT_LCD_ENABLE       = 51;
const int VENT_LCD_RS           = 53;
const int VENT_LCD_DB4          = 47;
const int VENT_LCD_DB5          = 45;
const int VENT_LCD_DB6          = 43;
const int VENT_LCD_DB7          = 41;

//Digital Inputs
const int ALARM_SWITCH_PIN      = 18;
const int MODE_SWITCH_PIN       = 39;
const int LIMIT_SWITCH_PIN		  = 10;

// Alarm pins
const int ALARM_BUZZER_PIN = 11;
const int ALARM_LED_PIN    = 12;
const int ALARM_RELAY_PIN  = 13;

//Parameter Select Push Buttons
const int TIDAL_VOLUME_SELECT_PIN 		  = 4;
const int INSPIRATION_TIME_SELECT_PIN	  = 5;
const int BPM_SELECT_PIN				        = 6;
const int THRESHOLD_PRESSURE_SELECT_PIN = 7;
const int MODE_SELECT_PIN = 8;
const int HIGH_PIP_ALARM_SELECT_PIN     = 24;
const int LOW_PIP_ALARM_SELECT_PIN      = 22;
const int HIGH_PEEP_ALARM_SELECT_PIN    = 28;
const int LOW_PEEP_ALARM_SELECT_PIN     = 26;
const int LOW_PLATEAU_PRESSURE_ALARM_SELECT_PIN = 30;
const int HIGH_RESPIRATORY_RATE_ALARM_SELECT_PIN = 50; //TODO: Make sure this pin is correct

//Parameter Change Encoder Pins
const int PARAMETER_ENCODER_PIN_1 			    = 3;
const int PARAMETER_ENCODER_PIN_2			      = 2;
const int PARAMETER_ENCODER_PUSH_BUTTON_PIN = 19; 


//LED Pins (except for alarm)
const int INHALE_LED_PIN   = 39;
const int EXHALE_LED_PIN   = 41;
const int VC_MODE_LED_PIN  = 7;
const int AC_MODE_LED_PIN  = 9;


#endif
