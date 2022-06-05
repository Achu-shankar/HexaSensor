#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DS3231.h>

File file;  // test file
//const uint8_t SD_CS = 10; // SD chip select
//RTC_DS1307 RTC;  // define the Real Time Clock object
DS3231 Clock;
char timestamp[30];
bool h12Flag;
bool pmFlag;
bool century = false;

//------------------------------------------------------------------------------
// call back for file timestamps
void dateTime(uint16_t* date, uint16_t* time) {
  // DateTime now = RTC.now();
  // sprintf(timestamp, "%02d:%02d:%02d %2d/%2d/%2d \n", now.hour(),now.minute(),now.second(),now.month(),now.day(),now.year()-2000);
  // Serial.println("yy");
  // Serial.println(timestamp);
  
  // return date using FAT_DATE macro to format fields
   *date = FAT_DATE(Clock.getYear(), Clock.getMonth(century), Clock.getDate());
   // return time using FAT_TIME macro to format fields
   *time = FAT_TIME(Clock.getHour(h12Flag, pmFlag), Clock.getMinute(), Clock.getSecond());
}
//------------------------------------------------------------------------------
void setup() {
 
 Serial.begin(9600);
 Wire.begin();
// if (!RTC.begin()) {
//   Serial.println("RTC failed");
//   while(1);
// };
 // set date time callback function
 SdFile::dateTimeCallback(dateTime);
 
//  DateTime now = RTC.now();
//  sprintf(timestamp, "%02d:%02d:%02d %2d/%2d/%2d \n", now.hour(),now.minute(),now.second(),now.month(),now.day(),now.year()-2000);
//  Serial.println("xx");
//  Serial.println(timestamp);
 
 
 if (!SD.begin()) {
   Serial.println("SD.begin failed");
   while(1);
 }
 file = SD.open("TEST_SD.TXT", FILE_WRITE);
 file.println("Testing 1,2,3...");
 
 delay(5000);
 file.close();
 Serial.println("Done");
}
//------------------------------------------------------------------------------
void loop() {}
