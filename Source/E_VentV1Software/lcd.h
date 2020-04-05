/* LCD Support functions that were commented out in the main file.
 */

#ifndef lcd_h
#define lcd_h

//-------------------------------------------------------------------------------------
//LCD Support Functions...here be dragons...


/*Function to display the parameter change screen on the LCD
  Inputs:
  - temp values for TV, BPM, IERatio, ThresholdPressure
*/
/*
  void displayParameterScreen(float tempTV, float tempBPM, float tempIERatio, float tempThresholdPressure) {
  //Prep variable for output
  int lcdTV = roundAndCast(tempTV);
  int lcdBPM = roundAndCast(tempBPM);
  int lcdIERatio = roundAndCast(tempIERatio);
  int lcdThresholdPressure = roundAndCast(tempThresholdPressure);

  //Prep lines for output
  char parameterScreenL1[] = "PRESS SET TO CONFIRM";
  char parameterScreenL2[];
  char parameterScreenL3[];
  char parameterScreenL4[];
  sprintf(parameterScreenL2, "TV=%3d%%   BPM=%2d/MIN", lcdTV, lcdBPM); //Spacing checked
  sprintf(parameterScreenL3, "I:E=1:%1d", lcdIERatio);
  sprintf(parameterScreenL4, "AC_THRESHOLD=%2d MBAR", lcdThresholdPressure);//Spacing checked

  //Display data
  lcd.clear();
  lcd.print(parameterScreenL1);
  lcd.setCursor(0, 1);
  lcd.write(parameterScreenL2);
  lcd.setCursor(0, 2);
  lcd.write(parameterScreenL3);
  lcd.setCursor(0, 3);
  lcd.write(parameterScreenL4);
  }
*/
/*Function to display the ventilator (regular) screen on the LCD
  Inputs:
  - temp values for TV, BPM, IERatio, ThresholdPressure
  - Ventilaton mode
  - Pressure data for peak, plateau, peep
*/
/*
  void displayVentilationScreen(float intTV, float intBPM, float intIERatio, float intThresholdPressure, enum machineStates machineState, float peakPressure, float plateauPressure, float peepPressure) {

  //Prep variable for output
  int lcdTV = roundAndCast(intTV);
  int lcdBPM = roundAndCast(intBPM);
  int lcdIERatio = roundAndCast(intIERatio);
  int lcdThresholdPressure = roundAndCast(intThresholdPressure);
  int lcdPeakPressure = roundAndCast(peakPressure);
  int lcdPlateauPressure = roundAndCast(plateauPressure);
  int lcdPeepPressure = roundAndCast(peepPressure);

  char ventilatorScreenL1[];
  char ventilatorScreenL2[];
  char ventilatorScreenL3[];
  char ventilatorScreenL4[];

  //Prep first line
  if (machineState == ACMode) {
  sprintf(ventilatorScreenL1, "AC (ACT=%2d)    DATA:", lcdThresholdPressure); //Spacing checked
  }
  else if (machineState == VCMode) {
  sprintf(ventilatorScreenL1, "VC             DATA:"); //Spacing checked
  }
  else {
  sprintf(ventilatorScreenL1, "N/A        DATA:"); //Spacing checked
  }
  //Prep remaining lines
  sprintf(ventilatorScreenL2, "TV=%3d%%      PEAK:%2d", lcdTV, lcdPeakPressure); //Spacing checked
  sprintf(ventilatorScreenL3, "BPM=%2d/MIN   PLAT:%2d", lcdBPM, lcdPlateauPressure); //Spacing checked
  sprintf(ventilatorScreenL4, "I:E=%1d        PEEP=%2d", lcdIERatio, lcdPeepPressure); //Spacing checked

  //Display data
  lcd.clear();
  lcd.print(ventilatorScreenL1);
  lcd.setCursor(0, 1);
  lcd.write(ventilatorScreenL2);
  lcd.setCursor(0, 2);
  lcd.write(ventilatorScreenL3);
  lcd.setCursor(0, 3);
  lcd.write(ventilatorScreenL4);

  }
*/
/*Function to display start up screen on LCD
  Inputs:
  - current software version
*/
/*
  void displayStartScreen(const char softwareVersion[]) {
  const char splashScreen[] = "CALGARY E-VENT";
  lcd.clear();
  lcd.print(splashScreen);
  lcd.setCursor(0, 1);
  lcd.print(softwareVersion);
  delay(2000);
  }
*/
/*Function to display errors on LCD
  Inputs:
  - 8-bit error code number
  Bit   Error
  0     High Peak Pressure
  1     Low Peak Pressure/Disconnection
  2     High PEEP
  3     Low PEEP
  4     Apnea
*/
/*
  void displayErrorScreen(uint8_t error) {
  
  const char errorScreenL1[] = "ALARM CONDITIONS:";
  char errorScreenL2[]=" ";
  char errorScreenL3[]=" ";
  char errorScreenL4[]=" ";
  //Peak Pressure Alarms
  if (bitRead(error, 0) == 1) {
  sprintf(errorScreenL2, "PEAK HIGH");
  }
  if (bitRead(error, 1) == 1) {
  sprintf(errorScreenL2, "PEAK LOW/DISCONNECT");
  }
  if (bitRead(error, 2) == 1) {
  sprintf(errorScreenL3, "PEEP HIGH");
  }
  if (bitRead(error, 3) == 1) {
  sprintf(errorScreenL3, "PEEP LOW");
  }
  if (bitRead(error, 4) == 1) {
  sprintf(errorScreenL4, "APNEA");
  }
  lcd.clear();
  lcd.print(errorScreenL1);
  lcd.setCursor(0, 1);
  lcd.write(erroorScreenL2);
  lcd.setCursor(0, 2);
  lcd.write(errorScreenL3);
  lcd.setCursor(0, 3);
  lcd.write(errorScreenL4);
  }
*/
/*Function to display homing message on LCD
   
 */
/*
  void displayHomingScreen() {
  const char calibrationScreenL1[] = "Calibration in";
  const char calibrationScreenL2[] = "progress...";
  lcd.clear();
  lcd.print(calibrationScreenL1);
  lcd.setCursor(0, 1);
  lcd.print(calibrationScreenL2);
  }
*/
/*Function to display start up screen on LCD
  Inputs:
  - float

  Outputs:
  - a rounded integer
*/
/*
  int roundAndCast(float x) {
  int new_var;
  new_var = (int) round(x);
  }
*/

#endif
