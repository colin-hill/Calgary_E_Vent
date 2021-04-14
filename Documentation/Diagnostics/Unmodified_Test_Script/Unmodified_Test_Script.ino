const uint8_t sensor_pins[7] = {A1,A2,A3,A4,A5,A6,A7};
const uint8_t num_sensor_pins = 7;

const uint32_t sample_period_in_millis = 100;

uint32_t initial_time;
uint32_t current_time;

void setup() {
  Serial.begin(9600);

  for(uint8_t i = 0; i < num_sensor_pins; i++){
    pinMode(sensor_pins[i],INPUT);
  }

  initial_time = millis();
 
}

void loop() {
  current_time = millis();

  if(current_time - initial_time > sample_period_in_millis){

    for(uint8_t i = 0; i< num_sensor_pins; i++){

      float current_reading = 5*(analogRead(sensor_pins[i])/1024.0); //10 bit ADC scaled to 5 volts
      current_reading = 360.0 / 5.0 * current_reading;
      Serial.print(i);
      Serial.print(": ");
      Serial.print(current_reading);
      if(i < num_sensor_pins - 1){
        Serial.print(",");
      }
  
    }

    Serial.println();
  }

}
