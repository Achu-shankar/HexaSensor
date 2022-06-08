void InitialisePressureSensor()
{
  if(!Ms5611.begin())
  {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(10);
  }
  // Get reference pressure for relative altitude
  reference_pressure = Ms5611.readPressure();
}
void UpdatePressTempSensorReading()
{  
  temperature_ms5611 = Ms5611.readTemperature();
  pressure_ms5611    = Ms5611.readPressure();
  absolute_altitude_ms5611 = Ms5611.getAltitude(pressure_ms5611);  
}
