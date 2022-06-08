void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  Serial.print((millis()/1000)) ; Serial.print(" == "); Serial.println(!((millis()/1000)%10));
  delay(500);

}
