#include <Wire.h>;
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#include "SerialTransfer.h"
//#include <SoftwareSerial.h>



#define RX_PIN 26
#define TX_PIN 25


#define TX_DATA_CHUNK_SIZE 28
#define DATA_ARR_SIZE 784
#define TA_SHIFT 8

//SoftwareSerial SoftSerial(RX_PIN, TX_PIN); 

SerialTransfer myTransfer;
paramsMLX90640 MLX90640;

struct payload_t {                   // Structure of our payload
  unsigned long msgID;
  unsigned long startIndx;
  unsigned long dataLen;
  float sensorData[TX_DATA_CHUNK_SIZE];
};



int minNoOfTries = 2;
int loopID = 1;
float dataArray[DATA_ARR_SIZE];

// IR cam vars
const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640
static float mlx90640To[768] = {0};

void setup()
{
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);
//  SoftSerial.begin(57600);
  Serial2.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);
  myTransfer.begin(Serial2);

  IRCamInitConfig();
}


void loop()
{
  float sum =0;
  // use this variable to keep track of how many
  // bytes we're stuffing in the transmit buffer
  uint16_t sendSize = 0;
  UpdateIRCameraReading();
  for (int i = 0; i<768; i++){
      dataArray[i] = mlx90640To[i];
  }
  for(int i=0; i<sizeof(dataArray)/sizeof(dataArray[0]);i++){
   sum += dataArray[i];
  }
  Serial.println(sum);
  for (int x = 0 ; x < 10 ; x++)
  {
    Serial.print("Pixel ");
    Serial.print(x);
    Serial.print(": ");
    Serial.print(mlx90640To[x], 2);
    Serial.print("C");
    Serial.println();
  }

  transmitData(dataArray, DATA_ARR_SIZE);
  loopID++;
  
  delay(100);
}

void transmitData(float arr[], int arrSize){
  payload_t payload;
  unsigned long startIndx = 0;
  int noOfChunks     = (int) arrSize/TX_DATA_CHUNK_SIZE;
  int remainderChunk = (int) arrSize%TX_DATA_CHUNK_SIZE;
  bool send_ack;
  uint16_t sendSize = 0;
  


  for(int j=1; j<=noOfChunks-1; j++){
    payload.startIndx = startIndx;
    payload.dataLen   = TX_DATA_CHUNK_SIZE;
    payload.msgID     = loopID;
    
    for(int i=0; i<TX_DATA_CHUNK_SIZE;i++){
      payload.sensorData[i] = arr[i+startIndx];
    }

     myTransfer.sendDatum(payload);
     startIndx =  j*TX_DATA_CHUNK_SIZE;

     
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
