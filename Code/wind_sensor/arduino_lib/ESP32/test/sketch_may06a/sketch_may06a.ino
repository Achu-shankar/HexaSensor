#include <SoftwareSerial.h>

#define RX_PIN 26
#define TX_PIN 25
SoftwareSerial SoftSerial(RX_PIN, TX_PIN); 

String txt = "typing";
String rctxt = "";
void setup() {
  SoftSerial.begin(115200);
  Serial.begin(115200);
  // put your setup code here, to run once:

}

void loop() {
  SoftSerial.print(txt);
  

}

//void loop() {
//  while (SoftSerial.available()){
//    char c = SoftSerial.read();
//    rctxt += c;
//    
//  }
//  Serial.print(rctxt);
//  
//}
