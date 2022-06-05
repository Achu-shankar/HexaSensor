void WriteSensorReadingToMemCard()
{
  String tempStr;

  
//  SensorDataFile = SD.open(filename, FILE_APPEND);
  tempStr = timeStampMemoryCard+",";
  WriteToFile(SensorDataFile,tempStr);

//  tempStr = String(wind_inp_buffer_temp)+",";
//  WriteToFile(SensorDataFile,tempStr);  

  
  tempStr = String(wind_speed,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(wind_angle,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(temperature_acoustic_wind_sensor,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(temp_reading_validity)+",";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = String(temperature_aht10,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(relative_humidity_aht10,4)+",";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = String(temperature_ms5611,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(pressure_ms5611)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(absolute_altitude_ms5611,4)+",";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = String(roll,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(pitch,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(yaw,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(rollspeed,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(pitchspeed,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(yawspeed,4)+",";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = String(x,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(y,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(z,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(vx,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(vy,4)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(vz,4)+",";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = String(pix_lat)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(pix_lon)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(pix_alt)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(pix_gps_vx)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(pix_gps_vy)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(pix_gps_vz)+",";
  WriteToFile(SensorDataFile,tempStr);
  
  tempStr = String(latitude,7)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(longitude,7)+",";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = String(altitude_gps,7)+",";
  WriteToFile(SensorDataFile,tempStr);
 
  if (NewDataRcvd){
    for (int x = 0 ; x < 768 ; x++)
    {
      tempStr = String(IrCamDataArray[x],4)+",";
      WriteToFile(SensorDataFile,tempStr);
    }
    NewDataRcvd = false;
  }

  SensorDataFile.println();
  
//  SensorDataFile.flush();

  
//  SensorDataFile.close();
  
}

void IntialiseSDCard(){

  if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }

    
  createDir(SD, "/sensor_outputs");
  if(!SD.exists(SYSINIT_DAT)){
    CreateSysInitFile();
  }
  else {
    UpdateSysInitFile();
  }
  filename = logFileName();
  Serial.print("Filename: "); Serial.println(filename);
  SensorDataFile = SD.open(filename, FILE_WRITE);
//  SensorDataFile.close();
  AddFileColHeadings();
}

void AddFileColHeadings(){
  String tempStr;
  
  tempStr = "Timestamp,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Wind speed,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Wind angle,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Acoustic temperature windsensor,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Temperature validity,";
  WriteToFile(SensorDataFile,tempStr);
  
  tempStr = "Temperature_aht10,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Relative humidity_aht10,";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = "Temperature_ms5611,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Pressure_ms5611,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Absolute_altitude_ms5611,";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = "Roll,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Pitch,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Yaw,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Roll speed,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "Pitch speed,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr ="Yaw speed,";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = "x,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "y,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "z,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "vx,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "vy,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "vz,";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = "pix_lat,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "pix_lon,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "pix_alt,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "pix_gps_vx,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "pix_gps_vy,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "pix_gps_vz,";
  WriteToFile(SensorDataFile,tempStr);

  tempStr = "latitude,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "longitude,";
  WriteToFile(SensorDataFile,tempStr);
  tempStr = "altitude_gps,";
  WriteToFile(SensorDataFile,tempStr);

  for (int x = 0 ; x < 768 ; x++)
  {
    tempStr = "Pixel_" +  String(x)+",";
    WriteToFile(SensorDataFile,tempStr);
  }
  SensorDataFile.println();
  
}

void WriteToFile(File file, String val){
  file.print(val);
}
void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(!fs.exists(path)){
      if(fs.mkdir(path)){
          Serial.println("Dir created");
      } else {
          Serial.println("mkdir failed");
      }
    }
    else{
      Serial.println("Directory already exists!");
    }
}




void CreateSysInitFile(){
  sysinit.fileName = 1;
  File file = SD.open(SYSINIT_DAT, FILE_WRITE);
  if (file) {
    file.seek(0); // Overwrite existing content instead of appending
    file.write((uint8_t*)&sysinit, sizeof(sysinit_t));
    file.close();
  }
  else{
    Serial.println("Failed to create init file");
  }
}
void UpdateSysInitFile(){
  
  File fileNameIndxFile  = SD.open(SYSINIT_DAT);
  if (fileNameIndxFile){
   uint8_t* pSysinit = (uint8_t*)&sysinit;
      while(fileNameIndxFile.available()) {
        *pSysinit++ = fileNameIndxFile.read();
      }
      fileNameIndxFile.close();
  }
  sysinit.fileName++;
  
  File file = SD.open(SYSINIT_DAT, FILE_WRITE);
  if (file) {
    file.seek(0); // Overwrite existing content instead of appending
    file.write((uint8_t*)&sysinit, sizeof(sysinit_t));
    file.close();
  }
  
}
char logFileNameBuf[64] = {0};
//String logFileNameBuf = "";

char* logFileName() {
  
  if (logFileNameBuf[0] == 0x0) {
    initLogFileName();
  }

  return logFileNameBuf;
}

void initLogFileName() {
  String timeStampFileName = "";
  updateTime();
  timeStampFileName = String(Date) + "_" + String(Month) + "_" + String(Year) 
                      +"_"+ String(Hour) + "_" + String(Minute) + "_" + String(Second);
  Serial.println(timeStampFileName);
  timeStampFileName = timeStampFileName + ".csv";

//  logFileNameBuf = "/sensor_outputs/" + timeStampFileName;
                      
  strcpy(logFileNameBuf,"/sensor_outputs/");
//  itoa(sysinit.fileName, &logFileNameBuf[16], 10);
  strcpy(&logFileNameBuf[strlen(logFileNameBuf)], timeStampFileName.c_str());
}




void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    Serial.println();
    file.close();
}
