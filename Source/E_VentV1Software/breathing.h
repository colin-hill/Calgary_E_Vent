/* Constants related to breathing, as well as some conversion functions.
 */

#ifndef breathing_h
#define breathing_h




const float SECONDS_PER_MINUTE = 60.0;

// Threshold Pressure Definitions--------------------------------------------------

const float MIN_THRESHOLD_PRESSURE     = 1.0; //cmH2O
const float MAX_THRESHOLD_PRESSURE     = 5.0; //cmH2O
const float DEFAULT_THRESHOLD_PRESSURE = 2.0; //cmH2O
//-------------------------------------------------------------------------------

// Breath Per Minute Definitions-------------------------------------------------
const float MIN_BPM     = 10.0; //Breaths per Minute, awful global for a reason
const float MAX_BPM     = 40.0; //Breaths per Minute, awful global for a reason
const float DEFAULT_BPM = 16.0; //Breaths per Minute
//------------------------------------------------------------------------------

// Tidal Volume Definitions------------------------------------------------------
const float MIN_TIDAL_VOLUME     = 5.0; //Tidal Volume (% of max)
const float MAX_TIDAL_VOLUME     = 100.0; //Tidal Volume (% of max)
const float DEFAULT_TIDAL_VOLUME = 50.0; //Tidal Volume (% of max)
//------------------------------------------------------------------------------

// Inspiration Expiration Ratio Definitions--------------------------------------
const float MIN_INSPIRATION_TIME     = 0.2; //Seconds
const float MAX_INSPIRATION_TIME     = 3.0; //Seconds
const float DEFAULT_INSPIRATION_TIME = 1.5; //Seconds
//------------------------------------------------------------------------------

//Breath hold time--------------------------------------------------------------
const float MIN_PLATEAU_PAUSE_TIME     = 0.10; //Seconds
const float MAX_PLATEAU_PAUSE_TIME     = 0.50; //Seconds
const float DEFAULT_PLATEAU_PAUSE_TIME = 0.15; //Seconds
//Max & Min PIP Pressures--------------------------------------------------------
const float MAX_PRESSURE = 40.0; //cmH2O
const float MIN_PRESSURE = 0.0; //cmH2O

//Max & Min PEEP Pressures-----------------------------------------------------------
const float MAX_PEEP_PRESSURE = 30.0; //cmH2O
const float MIN_PEEP_PRESSURE = 0.0; //cmH2O
//------------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Constants for taking measurements.
// ----------------------------------------------------------------------

//Potenitometer Definitions-----------------------------------------------------
const float BPM_POT_MAX_VOLTAGE       = 5.0;
const float IE_RATIO_POT_MAX_VOLTAGE  = 5.0;
const float TV_POT_MAX_VOLTAGE        = 5.0;
//------------------------------------------------------------------------------


// ----------------------------------------------------------------------
// Conversion functions.
// ----------------------------------------------------------------------

/*Function to convert the breaths per minute potentiometer voltage to a desired breaths per minute
  * Inputs:
  *    -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function
  *
  * Outputs:
  *    -set breaths per mintue
  */
float voltageToBPMConversion(const float potVoltage);


/*Function to convert inspiration - expiration ratio potentiometer voltage to a desired IE ratio
  * Inputs:
  *    -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function
  *
  * Outputs:
  *    -set IE ratio
  */
float voltageToIERatioConversion(const float potVoltage);


/*Function to convert tidal volume percentage potentiometer voltage to a desired tidal volume percentage
  * Inputs:
  *    -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function
  *
  * Outputs:
  *    -set tidal volume percentage
  */
float voltageToTVConversion(const float potVoltage);

#endif
