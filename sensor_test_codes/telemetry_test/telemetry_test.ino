#include <SoftwareSerial.h>

SoftwareSerial softSerial(35,27);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  softSerial.begin(57600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("hello");
  softSerial.println("hello");
  delay(5);

}
