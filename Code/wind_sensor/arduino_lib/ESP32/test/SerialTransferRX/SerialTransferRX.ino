#include "SerialTransfer.h"
//#include <SoftwareSerial.h>

#define RX_PIN 26
#define TX_PIN 25

#define TX_DATA_CHUNK_SIZE 28
#define DATA_ARR_SIZE 784

//SoftwareSerial SoftSerial(RX_PIN, TX_PIN); 

SerialTransfer myTransfer;
bool irDataRcved = false;

struct payload_t {                   // Structure of our payload
  unsigned long msgID;
  unsigned long startIndx;
  unsigned long dataLen;
  float sensorData[TX_DATA_CHUNK_SIZE];
};
float dataArray[DATA_ARR_SIZE];

TaskHandle_t Task1;

void setup()
{
  Serial.begin(230400);
//  SoftSerial.begin(57600);
  Serial2.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);
  myTransfer.begin(Serial2);

  xTaskCreatePinnedToCore(
             Task1code, /* Task function. */
             "Task1",   /* name of task. */
             10000,     /* Stack size of task */
             NULL,      /* parameter of the task */
             0  ,         /* priority of the task */
             &Task1,   /* Task handle to keep track of created task */ //);
             0); 

}


void loop()
{
  float sum=0;
  Serial.print("outside\n");
  for(int i=0; i<DATA_ARR_SIZE;i++){
       sum += dataArray[i];
      }
     Serial.println(sum);
  delay(500);
 
  
}

void Task1code( void * pvParameters ){
  

  for(;;){
    float sum=0;
  if(myTransfer.available())
  {
    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint16_t recSize = 0;

//    recSize = myTransfer.rxObj(testStruct, recSize);
//    Serial.print(testStruct.z);
//    Serial.print(testStruct.y);
//    Serial.print(" | ");
//    Serial.print(recSize);
//    Serial.print(" | ");
    
    payload_t payload;
    recSize = myTransfer.rxObj(payload, recSize);
    for(int i=0;i<payload.dataLen;i++){
      dataArray[i+payload.startIndx] = payload.sensorData[i];
    }
//    Serial.print("start Indx :"); Serial.println(payload.startIndx);
//    Serial.print("MSG ID :"); Serial.println(payload.msgID);
//    Serial.print("data length :"); Serial.println(payload.dataLen);

//    recSize = myTransfer.rxObj(dataArray, recSize);
    
//    Serial.println();
     if(payload.startIndx == DATA_ARR_SIZE-TX_DATA_CHUNK_SIZE){
      for (int i=0; i<10; i++){
         Serial.println(dataArray[i]); Serial.print(": ");
      }
//    Serial.println();
//      for(int i=0; i<DATA_ARR_SIZE;i++){
//       sum += dataArray[i];
//      }
//     Serial.println(sum);
      
    }
  }
  }
}
