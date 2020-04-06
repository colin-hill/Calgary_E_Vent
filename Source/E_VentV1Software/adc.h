/* Some constants for analog digital conversion
 */

#ifndef adc_h
#define adc_h

const float maxADCVoltage = 5.0;  //ATMega standard max input voltage
const int maxADCValue     = 1024; //ATMega standard 10-bit ADC

// Convenient conversion factor
const float ADC_READING_TO_VOLTS_FACTOR = maxADCVoltage / ((float)maxADCValue);

#endif
