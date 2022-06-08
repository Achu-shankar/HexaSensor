#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

#define TX_DATA_CHUNK_SIZE 28
#define DATA_ARR_SIZE 56

//RF24 radio(8,10); 
RF24 radio(14,12);               // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);     // Network uses that radio
const uint16_t this_node = 00;  // Address of our node in Octal format (04, 031, etc)
const uint16_t other_node = 01; // Address of the other node in Octal format


struct payload_t {                   // Structure of our payload
  unsigned long msgID;
  unsigned long startIndx;
  unsigned long dataLen;
  float sensorData[TX_DATA_CHUNK_SIZE];
};
float dataArray[DATA_ARR_SIZE]={1};
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF24Network/examples/helloworld_rx/"));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      // hold in infinite loop
    }
  }
  radio.setChannel(90);
  network.begin(/*node address*/ this_node);
//  pinMode(5,OUTPUT);
//  digitalWrite(5,HIGH);

}

void loop() {
 
  network.update();                  // Check the network regularly

  while (network.available()) {      // Is there anything ready for us?
 
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header, &payload, sizeof(payload));
    for(int i=0;i<payload.dataLen;i++){
      dataArray[i+payload.startIndx] = payload.sensorData[i];
//      Serial.print(i+payload.startIndx); Serial.print(",");
    }
    for(int i=0;i<9;i++){
     Serial.print("a"+String(i) + ":");
    Serial.print(dataArray[i]);
    Serial.print(",");
  }
//  Serial.print("a:");
//  Serial.print(dataArray[0]);
  Serial.println();
  }

//for(int i=0;i<9;i++){
//     Serial.print("a:");
//    Serial.print(dataArray[i]);
//    Serial.print(",");
//  }
//  Serial.println();
// delay(200);
}
