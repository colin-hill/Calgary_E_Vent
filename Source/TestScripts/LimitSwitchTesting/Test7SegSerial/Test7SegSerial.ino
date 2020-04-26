HardwareSerial & ExtDisplay = Serial3;

float peep = 12.56;

int peep_to_display = (int) round(peep);

char testString[] = "HIrr";

void setup() {
  // put your setup code here, to run once:



//ExtDisplay.write(0x7A); // Brightness control command
//ExtDisplay.write(100);



//ExtDisplay.print(testString);

//delay(50000);




}

void loop() {
  // put your main code here, to run repeatedly:

  ExtDisplay.begin(9600);

  ExtDisplay.write('v');
  ExtDisplay.print(testString);
  delay(3000);
  ExtDisplay.write('v');
  ExtDisplay.print(peep_to_display);
  delay(3000);

}
