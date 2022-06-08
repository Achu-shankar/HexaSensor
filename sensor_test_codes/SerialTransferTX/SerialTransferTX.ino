#include "SerialTransfer.h"
//#include <SoftwareSerial.h>


#define RX_PIN 26
#define TX_PIN 25


#define TX_DATA_CHUNK_SIZE 28
#define DATA_ARR_SIZE 784
#define TA_SHIFT 8

//SoftwareSerial SoftSerial(RX_PIN, TX_PIN); 

SerialTransfer myTransfer;

struct payload_t {                   // Structure of our payload
  unsigned long msgID;
  unsigned long startIndx;
  unsigned long dataLen;
  float sensorData[TX_DATA_CHUNK_SIZE];
};



int minNoOfTries = 2;
int loopID = 1;
float dataArray[DATA_ARR_SIZE];

void setup()
{
  Serial.begin(230400);
//  SoftSerial.begin(57600);
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  myTransfer.begin(Serial2);

 
}


void loop()
{
  // use this variable to keep track of how many
  // bytes we're stuffing in the transmit buffer
  uint16_t sendSize = 0;
  populateDataArray();
//  for (int i=0; i<10; i++){
//    Serial.print(dataArray[i]); Serial.print(": ");
//  }
//  Serial.println();

  ///////////////////////////////////////// Stuff buffer with struct
//  sendSize = myTransfer.txObj(testStruct, sendSize);

  ///////////////////////////////////////// Stuff buffer with array
//  sendSize = myTransfer.txObj(dataArray, sendSize);
//
//  ///////////////////////////////////////// Send buffer
//  myTransfer.sendData(sendSize);

  transmitData(dataArray, DATA_ARR_SIZE);
  loopID++;
  
  delay(10);
}

void transmitData(float arr[], int arrSize){
  payload_t payload;
  unsigned long startIndx = 0;
  int noOfChunks     = (int) arrSize/TX_DATA_CHUNK_SIZE;
  int remainderChunk = (int) arrSize%TX_DATA_CHUNK_SIZE;
  bool send_ack;
  uint16_t sendSize = 0;
  


  for(int j=1; j<=noOfChunks-1; j++){
//    Serial.print("start Indx :"); Serial.println(startIndx);
    payload.startIndx = startIndx;
    payload.dataLen   = TX_DATA_CHUNK_SIZE;
    payload.msgID     = loopID;
    
    for(int i=0; i<TX_DATA_CHUNK_SIZE;i++){
      payload.sensorData[i] = arr[i+startIndx];
    }
//     sendSize = myTransfer.txObj(payload, sendSize);
//     myTransfer.sendData(sendSize);
     myTransfer.sendDatum(payload);
     startIndx =  j*TX_DATA_CHUNK_SIZE;
//     Serial.print("j  : "); Serial.println(j);
     
  }
    
  
  
  // send final chunk separately
  payload.startIndx = startIndx;
  payload.dataLen   = TX_DATA_CHUNK_SIZE;
  payload.msgID     = loopID;
  
  for(int i=0; i<TX_DATA_CHUNK_SIZE;i++){
    payload.sensorData[i] = arr[i+startIndx];
  }
//   sendSize = myTransfer.txObj(payload, sendSize);
//   myTransfer.sendData(sendSize);
   myTransfer.sendDatum(payload);
//   delay(50);

   // send final chunk separately
  
  
  
}



void populateDataArray(){
  float sum=0;
  for(int i=0; i<sizeof(dataArray)/sizeof(dataArray[0]);i++){
    dataArray[i] = (float) random(0,1000)/100.0;
  }

  for(int i=0; i<sizeof(dataArray)/sizeof(dataArray[0]);i++){
   sum += dataArray[i];
  }
  Serial.println(sum);
  
}
