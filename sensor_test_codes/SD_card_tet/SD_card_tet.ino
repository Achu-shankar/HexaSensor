
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SYSINIT_DAT "/sysinit.txt"

// Type for system initialization variables
typedef struct {
  // Arduino boot counter (incremented in every boot)
  uint16_t bootCount;

  // Variable for unique data file name on SD card
  uint16_t fileName;
} sysinit_t;

// Variable that holds system initialization data
sysinit_t sysinit;

char* filename;
File SensorDataFile;  


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
//  IntialiseSDCard();  
//  WriteToMemCard();
//  SensorDataFile.close();
  filename = "/65.csv";
  readFile(SD, filename);

  listDir(SD, "/", 1);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void WriteToMemCard()
{
  if(SensorDataFile.print("Sample")){
    Serial.println("Written to file");
  }
  else {
    Serial.println("Failed to write to file");
  }
  SensorDataFile.flush();
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


void IntialiseSDCard(){
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
char logFileNameBuf[32] = {0};

char* logFileName() {
  
  if (logFileNameBuf[0] == 0x0) {
    initLogFileName();
  }

  return logFileNameBuf;
}

void initLogFileName() {
  strcpy(logFileNameBuf,"/sensor_outputs");
  logFileNameBuf[15] = '/';
  itoa(sysinit.fileName, &logFileNameBuf[16], 10);
  strcpy(&logFileNameBuf[strlen(logFileNameBuf)], ".csv");
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
