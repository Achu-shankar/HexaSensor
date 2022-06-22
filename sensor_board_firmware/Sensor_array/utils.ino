void combineSensorData(){
  sensorDataArray[0] = (float) wind_speed;
  sensorDataArray[1] = (float) wind_angle;

  sensorDataArray[2] = (float) temperature_aht10;
  sensorDataArray[3] = (float) relative_humidity_aht10;

  sensorDataArray[4] = (float) temperature_ms5611;
  sensorDataArray[5] = (float) pressure_ms5611;
  sensorDataArray[6] = (float) absolute_altitude_ms5611;

  sensorDataArray[7] = (float) latitude;
  sensorDataArray[8] = (float) longitude;
  sensorDataArray[9] = (float) altitude_gps;

//  for (int i = 0; i<768; i++){
//      sensorDataArray[i+10] = mlx90640To[i];
//  }
}

void PrintValues()
{
//  Serial.print("Wind Speed text: ");Serial.println(wind_inp_buffer_temp);
//  Serial.print("Wind Speed: "); Serial.println(wind_speed);
//  Serial.print("Wind Angle: "); Serial.println(wind_angle);
//  Serial.print("Temp acoustic wind sensor: "); Serial.println(temperature_acoustic_wind_sensor);
  if (wind_data_validity){
    Serial.print("Wind speed validity: "); Serial.println(wind_data_validity);
    wind_data_validity = false;
  }
  
//  Serial.print("Wind sensor status: "); Serial.println(wind_sensor_status);
//  Serial.print("Temp reading validity: "); Serial.println(temp_reading_validity);
    

//////  
//  Serial.print("Temp aht10: "); Serial.println(temperature_aht10);
//  Serial.print("Rel humidity: "); Serial.println(relative_humidity_aht10);
////  
//  Serial.print("Temp ms5611: ");  Serial.println(temperature_ms5611);
//  Serial.print("Press ms5611: "); Serial.println(pressure_ms5611);
//  Serial.print("Abs altitude: "); Serial.println(absolute_altitude_ms5611);
  
//  Serial.print("Latitude: ");     Serial.println(latitude);
//  Serial.print("Longitude: ");    Serial.println(longitude);
//  Serial.print("Altitude gps: "); Serial.println(altitude_gps);
//
//  Serial.print("pix_lat: ");     Serial.println(pix_lat);
//  Serial.print("pix_lon: ");    Serial.println(pix_lon);
//  Serial.print("pix_alt: "); Serial.println(pix_alt);

//  Serial.print("roll: ");     Serial.println(roll);
//  Serial.print("pitch: ");    Serial.println(pitch);
//  Serial.print("yaw: "); Serial.println(yaw);
//
//  for (int x = 0 ; x < 3 ; x++)
//  {
//    Serial.print("Pixel: ");
//    Serial.print(x);
//    Serial.print(": ");
//    Serial.print(IrCamDataArray[x], 2);
//    Serial.print("C");
//    Serial.println();
//  }

//  Serial.print(Hour);Serial.print(",");Serial.println(Second);
//Serial.print("TimeStamp");Serial.println(timeStampMemoryCard);
//  Serial.print(initMillis); Serial.print(":"); Serial.print(initTime); Serial.print(":"); Serial.print(Hour);Serial.print(":"); 
//  Serial.println(MilliSecond);
  
}
