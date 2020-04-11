const int LIMIT_SWITCH_PIN = 10;
const int ALARM_BUZZER_PIN = 11;

void setup(){

	pinMode(LIMIT_SWITCH_PIN,INPUT_PULLUP);
	pinMode(ALARM_BUZZER_PIN, OUTPUT);
	

}

void loop(){

	if(LOW == digitalRead(LIMIT_SWITCH_PIN)){
		digitalWrite(ALARM_BUZZER_PIN,HIGH);
	}
	else{
		digitalWrite(ALARM_BUZZER_PIN,LOW);
	}
	
}