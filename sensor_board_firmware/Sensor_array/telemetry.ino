//void IntialiseTelemetry()
//{
//   if (!radio.begin()) {
//    Serial.println(F("Radio hardware not responding!"));
//  }
//
//  radio.setChannel(90);
//  network.begin(ADDRESS_THIS_NODE);
//  
//}
//
//void TransmitSensorReading(){
//  combineSensorData();
//  transmitData(sensorDataArray, DATA_ARR_SIZE);
//}
//
//void transmitData(float arr[], int arrSize){
//  payload_t payload;
//  int startIndx = 0;
//  int noOfChunks     = (int) arrSize/TX_DATA_CHUNK_SIZE;
//  int remainderChunk = (int) arrSize%TX_DATA_CHUNK_SIZE;
//  bool send_ack;
//  RF24NetworkHeader header(/*to node*/ ADDRESS_BASE_NODE);
//
//  for(int j=1; j<=noOfChunks-1; j++){
//    payload.startIndx = startIndx;
//    payload.dataLen   = TX_DATA_CHUNK_SIZE;
//    payload.msgID     = loopID;
//    
//    for(int i=0; i<TX_DATA_CHUNK_SIZE;i++){
//      payload.sensorData[i] = arr[i+startIndx];
//    }
//    for (int retry = 0; retry<noOfTransReTries;retry++){
//      send_ack = network.write(header, &payload, sizeof(payload));
//      if(send_ack){
//        Serial.print("Send chunk: "); Serial.println(j);
//        break;
//      }
//    }
//     
//    startIndx = j*TX_DATA_CHUNK_SIZE;
//    
//  }
//  
//  // send final chunk separately
//  payload.startIndx = startIndx;
//  payload.dataLen   = TX_DATA_CHUNK_SIZE;
//  payload.msgID     = loopID;
//  
//  for(int i=0; i<remainderChunk;i++){
//    payload.sensorData[i] = arr[i+startIndx];
//  }
//  for (int retry = 0; retry<noOfTransReTries;retry++){
//    send_ack = network.write(header, &payload, sizeof(payload));
//    if(send_ack){
//      break;
//    }
//  }
//  
//}

void TransmitSensorReading(){
  

  telemetrySerial.print(wind_speed); telemetrySerial.print(",");
  telemetrySerial.print(wind_angle); telemetrySerial.print(",");
  telemetrySerial.print(temperature_acoustic_wind_sensor); telemetrySerial.print(",");

  telemetrySerial.print(temperature_ms5611); telemetrySerial.print(",");
  telemetrySerial.print(pressure_ms5611); telemetrySerial.print(",");
  telemetrySerial.print(absolute_altitude_ms5611); telemetrySerial.print(",");

  telemetrySerial.print(temperature_aht10);       telemetrySerial.print(",");
  telemetrySerial.print(relative_humidity_aht10); telemetrySerial.print(",");

  telemetrySerial.print(time_boot_ms_attitude);telemetrySerial.print(",");
  telemetrySerial.print(roll); telemetrySerial.print(",");
  telemetrySerial.print(pitch); telemetrySerial.print(",");
  telemetrySerial.print(yaw); telemetrySerial.print(",");

  telemetrySerial.print(rollspeed); telemetrySerial.print(",");
  telemetrySerial.print(pitchspeed); telemetrySerial.print(",");
  telemetrySerial.print(yawspeed); telemetrySerial.print(",");

  telemetrySerial.print(time_boot_ms_loc_pos);telemetrySerial.print(",");
  telemetrySerial.print(x); telemetrySerial.print(",");
  telemetrySerial.print(y); telemetrySerial.print(",");
  telemetrySerial.print(z); telemetrySerial.print(",");

  telemetrySerial.print(vx); telemetrySerial.print(",");
  telemetrySerial.print(vy); telemetrySerial.print(",");
  telemetrySerial.print(vz); telemetrySerial.print(",");

  telemetrySerial.print(time_boot_ms_pix_gps);telemetrySerial.print(",");
  telemetrySerial.print(pix_lat); telemetrySerial.print(",");
  telemetrySerial.print(pix_lon); telemetrySerial.print(",");
  telemetrySerial.print(pix_alt); telemetrySerial.print('\n');

//  telemetrySerial.println(timeStampMemoryCard);
   
  
}
