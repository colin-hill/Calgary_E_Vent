#ifndef PinAssignments_h
#define PinAssignments_h


//All constants should be written like VARIABLE_NAME

//TODO: assign actual pins

//LCD Pins
const int ALARM_LCD_ENABLE      = 37;
const int ALARM_LCD_RS          = 23;
const int ALARM_LCD_DB4         = 27;
const int ALARM_LCD_DB5         = 29;
const int ALARM_LCD_DB6         = 31;
const int ALARM_LCD_DB7         = 33;

const int VENT_LCD_ENABLE       = 25;
const int VENT_LCD_RS           = 23;
const int VENT_LCD_DB4          = 27;
const int VENT_LCD_DB5          = 29;
const int VENT_LCD_DB6          = 31;
const int VENT_LCD_DB7          = 33;

const int ALARM_BACKLIGHT		= 43;
const int VENT_BACKLIGHT		= 35;

//Digital Inputs
const int ALARM_SWITCH_PIN      = 19;
const int MODE_SWITCH_PIN       = 8;
const int LIMIT_SWITCH_PIN		= 34;


// Alarm pins
const int ALARM_BUZZER_PIN = 42;
const int ALARM_LED_PIN    = 40; //Originally 40


//Parameter Select Push Buttons
const int TIDAL_VOLUME_SELECT_PIN 		  = 26;
const int INSPIRATION_TIME_SELECT_PIN	  = 28;
const int BPM_SELECT_PIN				  = 24;
const int THRESHOLD_PRESSURE_SELECT_PIN   = 32; 
const int MODE_SELECT_PIN 				  = 8; 
const int HIGH_PIP_ALARM_SELECT_PIN       = 49;
const int LOW_PIP_ALARM_SELECT_PIN        = 53;
const int HIGH_PEEP_ALARM_SELECT_PIN      = 51;
const int LOW_PEEP_ALARM_SELECT_PIN       = 52;
const int LOW_PLATEAU_PRESSURE_ALARM_SELECT_PIN = 36; //Changed to avoid conflict, is no longer used
const int HIGH_RESPIRATORY_RATE_ALARM_SELECT_PIN = 50; 

//Parameter Change Encoder Pins
const int PARAMETER_ENCODER_PIN_1 			= 2;
const int PARAMETER_ENCODER_PIN_2			= 3;
const int PARAMETER_ENCODER_PUSH_BUTTON_PIN = 18; 


//LED Pins (except for alarm)
const int INHALE_LED_PIN   = 39; // Originally 39
const int EXHALE_LED_PIN   = 41; //Originally 41
const int VC_MODE_LED_PIN  = 7; //Originally 7
const int AC_MODE_LED_PIN  = 13; //Originally 9

//Recalibration pin
const int RECALIBRATION_PIN = 22;

//Absolute Encoder Pin
const int ABS_ENCODER_PIN = 5; //Compiler will call A5


#endif
