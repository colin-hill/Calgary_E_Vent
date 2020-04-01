#define setParameterPin 3
#define pressureSensorPin 4
#define setBPMPotPin
#define setIERatioPotPin
#define setPressurePotPin
#define setTVPotPin

#define maxADCVoltage 5.0
#define maxADCValue 1024

#define minPressurSensorVoltage 0.0
#define maxPressureSensorVoltage 5.0
#define minPressure 0.0 //Guage psi,cmH20?
#define maxPressure 3.0 //Guage psi, cmH20?

#define bpmPotMaxVoltage 5.0
#define ieRatioPotMaxVoltage 5.0
#define tvPotMaxVoltage 5.0
#define setPressurePotMaxVoltage 5.0

#define minBPM 10.0 //Breaths per Minute
#define maxBPM 40.0 //Breaths per Minute

#define minTV 0.0 //Tidal Volume (% of max)
#define maxTV 100.0 //Tidal Volume (% of max)

#define minIERatio 1.0 //Inspiration to Expiration ratio 1:1
#define maxIERatio 0.25 //Inspiration to Expiration ratio 1:4

float dispBPM; //Breaths per Minute Value displayed on LCD
float dispIE; //Inspiration Expiration Ratio Value displayed on LCD
float dispTV; //Tidal Volume displayed on LCD

volatile float internalBPM; //Breaths per Minute to be used on next breath cycle
volatile float internalIE; //I:E ratio to be used on the next breath cycle
volatile float internalTV; //Tidal Volume to be used on next breath cycle

float adcReadingToVoltsFactor = (maxADCVoltage - 0.0) / (maxADCValue - 0);


void setup() {
  pinMode(setPressurePotPin, INPUT);
  pinMode(setBPMPotPin, INPUT);
  pinMode(setIERatioPotPin, INPUT);
  pinMode(setTVPotPin, INPUT);

  uint16_t setPressurePotPinADCReading = analogRead(setPressurePotPin);
  uint16_t setBPMPotPinADCReading = analogRead(setBPMPotPin);
  uint16_t setIERatioPotPinADCReading = analogRead(setIERatioPotPin);
  uint16_t setTVPotPinADCReading = analogRead(setTVPotPin);

  float setPressurePotVoltage = setPressurePotPinADCReading * adcReadingToVoltsFactor;
  float setBPMPotVoltage = setBPMPotPinADCReading * adcReadingToVoltsFactor;
  float setIERatioVoltage = setIERatioPotPinADCReading * adcReadingToVoltsFactor;
  float setTVPotVoltage = setTVPotPinADCReading * adcReadingToVoltsFactor
}

void loop() {
  // put your main code here, to run repeatedly:

}

float voltageToPressureConversion(float sensorVoltage){

  float pressure = (maxPressure - minPressure) / (maxPressureVoltage - minPressureVoltage) * sensorVoltage;

  pressure += minPressure;

  return pressure;
}

float voltageToSetPressureConversion(float potVoltage){

  float setPressure = (maxSetPressure - minSetPressure) / (setPressurePotMaxVoltage - 0.0) * potVoltage;

  setPressure += minSetPressure;

  return setPressure;
  
}

float voltageToBPMConversion(float potVoltage){

  float BPM = (maxBPM - minBPM) / (bpmPotMaxVoltage - 0.0) * potVoltage;

  BPM += minBPM;

  return BPM;
}

float voltageToIERatioConversion(float potVoltage){

  float IERatio = (maxIERatio - minIERatio) / (ieRatioPotMaxVoltage - 0.0) * potVoltage;

  IERatio += minIERatio;

  return IERatio;
}

float voltageToTVConversion(float potVoltage){

  float TV = (maxTV - minTV) / (tvPotMaxVoltage - 0.0) * potVoltage;

  TV += minTV;

  return TV;
}
