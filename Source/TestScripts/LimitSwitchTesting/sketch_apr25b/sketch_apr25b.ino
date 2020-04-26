const int LED_1 = 7;
const int LED_2 = 9;
const int LED_3 = 10;
const int LED_4 = 11;
const int LED_5 = 13;

const int WAIT_PERIOD = 1000;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  digitalWrite(LED_4, HIGH);
  digitalWrite(LED_5, HIGH);
  delay(WAIT_PERIOD);
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  digitalWrite(LED_4, LOW);
  digitalWrite(LED_5, LOW);
  delay(WAIT_PERIOD);

}
