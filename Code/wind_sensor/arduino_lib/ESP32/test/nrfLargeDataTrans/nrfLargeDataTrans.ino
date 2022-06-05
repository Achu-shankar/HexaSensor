
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

#define TX_DATA_CHUNK_SIZE 28
#define DATA_ARR_SIZE 56


RF24 radio(14,12);   
//RF24 radio(8,10);                    // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;       // Address of our node in Octal format
const uint16_t other_node = 00;      // Address of the other node in Octal format

struct payload_t {                   // Structure of our payload
  unsigned long msgID;
  unsigned long startIndx;
  unsigned long dataLen;
  float sensorData[TX_DATA_CHUNK_SIZE];
};



int minNoOfTries = 2;
int loopID = 1;
float dataArray[DATA_ARR_SIZE];


void setup() {
   Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF24Network/examples/helloworld_tx/"));
  pinMode(5,OUTPUT);
  digitalWrite(5,LOW);

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      // hold in infinite loop
    }
  }
  
  radio.setChannel(90);
  network.begin(/*node address*/ this_node);

}

void loop() {
  network.update();
  populateDataArray();

   for(int i=0;i<20;i++){
    Serial.print(dataArray[i]);
  }
  Serial.println();
  
  transmitData(dataArray, DATA_ARR_SIZE);
  loopID++;
//  delay(200);
}

void transmitData(float arr[], int arrSize){
  payload_t payload;
  int startIndx = 0;
  int noOfChunks     = (int) arrSize/TX_DATA_CHUNK_SIZE;
  int remainderChunk = (int) arrSize%TX_DATA_CHUNK_SIZE;
  bool send_ack;
  RF24NetworkHeader header(/*to node*/ other_node);

  for(int j=1; j<=noOfChunks-1; j++){
    payload.startIndx = startIndx;
    payload.dataLen   = TX_DATA_CHUNK_SIZE;
    payload.msgID     = loopID;
    
    for(int i=0; i<TX_DATA_CHUNK_SIZE;i++){
      payload.sensorData[i] = arr[i+startIndx];
    }
    for (int retry = 0; retry<minNoOfTries;retry++){
      send_ack = network.write(header, &payload, sizeof(payload));
      if(send_ack){
//        Serial.print("Send chunk: "); Serial.println(j);
        break;
      }
    }
     
    startIndx = j*TX_DATA_CHUNK_SIZE;
    
  }
  
  // send final chunk separately
  payload.startIndx = startIndx;
  payload.dataLen   = TX_DATA_CHUNK_SIZE;
  payload.msgID     = loopID;
  
  for(int i=0; i<remainderChunk;i++){
    payload.sensorData[i] = arr[i+startIndx];
  }
  for (int retry = 0; retry<minNoOfTries;retry++){
    send_ack = network.write(header, &payload, sizeof(payload));
    if(send_ack){
      break;
    }
  }
  
}

void populateDataArray(){
  for(int i=0; i<sizeof(dataArray)/sizeof(dataArray[0]);i++){
    dataArray[i] = (float) random(0,1000)/100.0;
  }
}
