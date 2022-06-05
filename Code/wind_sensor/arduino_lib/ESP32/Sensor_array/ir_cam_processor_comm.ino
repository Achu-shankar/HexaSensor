
void UpdateIRCamData(){
float sum=0;
  if(IrCamSerialTransfer.available())
  {
    

    uint16_t recSize = 0;
    
    ircam_payload_t payload;
    recSize = IrCamSerialTransfer.rxObj(payload, recSize);
    for(int i=0;i<payload.dataLen;i++){
      IrCamDataArray[i+payload.startIndx] = payload.sensorData[i];
//      Serial.println("rcv data");
    }

     if(payload.startIndx == IRCAM_DATA_ARR_SIZE-IRCAM_TX_DATA_CHUNK_SIZE){
      NewDataRcvd = true;
//      for (int i=0; i<10; i++){
//         Serial.println(IrCamDataArray[i]); Serial.print(": ");
//      }
//      for(int i=0; i<IRCAM_DATA_ARR_SIZE;i++){
//       sum += IrCamDataArray[i];
//      }
//     Serial.println(sum);
      
    }
  }


}
