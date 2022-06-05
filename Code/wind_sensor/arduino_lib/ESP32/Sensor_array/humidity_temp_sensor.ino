
void InitialiseHumidityTempSensor(){
    if (! AHT.begin()) 
  {
    Serial.println("Could not find AHT? Check wiring");
//    while (1) delay(10);
  }
}
void UpdateHumTempSensorReading()
{
  AHT.getEvent(&humidity_aht10, &temp_aht10);
  temperature_aht10 = temp_aht10.temperature;
  relative_humidity_aht10 = humidity_aht10.relative_humidity;
}
