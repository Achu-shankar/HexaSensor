

void UpdateWindSensorReading()
{
  

  currentMillis = millis();
  if (currentMillis - prevMillis >= wind_sensor_reading_interval) {
//    Serial.print("inside\n");
//     Timing variables
     prevMillis = currentMillis;

    while (WindSerial.available() && !wind_have_new_data) {
  
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  
      wind_char = WindSerial.read();
//      Serial.print(wind_char);
      
      if (wind_char != wind_msg_term_char)
      {
        wind_inp_buffer[wind_indx] = wind_char;
        wind_indx = wind_indx + 1;
      }
      else
      {
        wind_inp_buffer[wind_indx] = '\0';
        wind_indx = 0;
        wind_have_new_data = true;
      }
  
     }
     wind_indx =0;
     digitalWrite(LED_BUILTIN, LOW);
     if (wind_have_new_data){
        strncpy(wind_inp_buffer_temp, wind_inp_buffer, 64);
        ParseWindSensorData();
        wind_have_new_data = false;
//        Serial.print("Wind Speed text: ");Serial.println(wind_inp_buffer_temp);
    }
    digitalWrite(WIND_RCV_ENABLE_PIN, HIGH);
    WindSerial.print(wind_query);
    digitalWrite(WIND_RCV_ENABLE_PIN, LOW);
  }
  
//  wind_have_new_data = false;
  

  
}

void ParseWindSensorData()
{
  char * strtokIndx;
  strtokIndx = strtok(wind_inp_buffer_temp,",="); 
  strtokIndx = strtok(NULL,",=");
  
  strtokIndx = strtok(NULL,",=");
  if(strtokIndx){
    wind_speed = atof(strtokIndx);
  }
   
  strtokIndx = strtok(NULL,",=");
  if(strtokIndx){
    wind_angle = atof(strtokIndx);
  }

  strtokIndx = strtok(NULL,",=");
  if(strtokIndx){
    wind_sensor_status = *strtokIndx;
  }
  
  strtokIndx = strtok(NULL,",=");
  if(strtokIndx){
    temperature_acoustic_wind_sensor = atof(strtokIndx);
  }
  
  strtokIndx = strtok(NULL,",=");
  strtokIndx = strtok(NULL,",=*");
  if(strtokIndx){
    temp_reading_validity = *strtokIndx;
  }
  
}
