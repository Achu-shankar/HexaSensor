#include <SoftwareSerial.h>

#define RX_PIN 26
#define TX_PIN 25
SoftwareSerial SoftSerial(RX_PIN, TX_PIN); 

//union floatToBytes {
//  
//    char buffer[4];
//    float voltageReading;
//  
//  } converter;
//
//
//
//
//void setup()
//{
//    Serial.begin(115200);
//    SoftSerial.begin(115200);
//    converter.voltageReading = 24.456;
//    SoftSerial.write(converter.buffer,4);
//}
//void loop()
//{
//}



//uint8_t indx = 0;
//
//void setup()
//{
//    
//    SoftSerial.begin(115200);
//    Serial.begin(115200);
//}
//void loop()
//{
//   
//    while (SoftSerial.available()){
//        converter.buffer[indx] = SoftSerial.read();
//        indx++;
//    }               
//    Serial.println(converter.voltageReading);    
//}

char ip;
void setup() {
  SoftSerial.begin(57600);
  Serial.begin(9600);

}

void loop()  
{ 
  SoftSerial.write("U");
  delay (100);
}

//void loop()  
//{ 
//  if (SoftSerial.available())
//  {
//    ip=SoftSerial.read();
//    Serial.print(ip);
//  }
//}



//
//void loop() {
//  
////  while(SoftSerial.available()){
////    Serial.print(SoftSerial.read());
////    Serial.println("receiving data");
////  }
//  
//  SoftSerial.print("3");
//  delay(200);
//
//}
