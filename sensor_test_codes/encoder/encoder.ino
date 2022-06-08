 
int encoder = 2;
 
volatile unsigned int counter;
int rpm;
 

void setup() {
  Serial.begin(9600);
  pinMode(encoder, INPUT);
  digitalWrite(encoder, HIGH);
 
  attachInterrupt(0,countpulse,RISING);
}
 
void countpulse(){
        counter++;
}
 
void loop() {
  static uint32_t previousMillis;
  if (millis() - previousMillis >= 1000) {
            rpm = (counter)*60;          
            counter = 0;
            previousMillis += 1000;
  }
 
 
  Serial.print("Speed: ");
  Serial.print(rpm);
  Serial.println(" rpm");
  delay(1);
}
