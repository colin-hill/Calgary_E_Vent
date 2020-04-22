HardwareSerial & ExtDisplay = Serial3;

float peep = 12.56;

int peep_to_display = (int) round(peep);

void setup() {
  // put your setup code here, to run once:

ExtDisplay.begin(9600);
ExtDisplay.write('v');

ExtDisplay.write(0x7A); // Brightness control command
ExtDisplay.write(100);


char testString[] = "HIrr";
ExtDisplay.print(testString);

delay(50000);




}

void loop() {
  // put your main code here, to run repeatedly:

  delay(1000);
  ExtDisplay.print(1);
  delay(1000);
  //ExtDisplay.write('v');
  ExtDisplay.print(peep_to_display);
  ExtDisplay.print(peep_to_display);
  delay(1000);
  ExtDisplay.write('v');
}
