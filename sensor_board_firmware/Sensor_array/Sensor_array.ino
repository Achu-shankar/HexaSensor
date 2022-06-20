
// --------------------------- Import libraries -----------------------------
#include <SoftwareSerial.h>
#include "SerialTransfer.h"
#include <Wire.h>;
#include <Adafruit_AHTX0.h>
#include <MS5611.h>;
#include <TinyGPS++.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#include "FS.h"
#include "SD.h"
//#include "SPI.h"
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <DS3231.h>
#include "mavlink.h"


// --------------------------- Define hardware pins ------------------------------
#define WIND_RCV_ENABLE_PIN 4
#define WIND_RX_PIN 16
#define WIND_TX_PIN 17
#define TELE_CE_PIN  14
#define TELE_CSN_PIN 12
#define IRCAM_RX_PIN 26
#define IRCAM_TX_PIN 25
//#define GPS_RX_PIN 26
//#define GPS_TX_PIN 25

#define pxSerial Serial1
//#define telemetrySerial Serial1


// --------------------------- Default  definitions -------------------------------
#define TA_SHIFT 8 //Default shift for MLX90640 in open air
#define SYSINIT_DAT "/sysinit.txt"
#define TX_DATA_CHUNK_SIZE 28
#define DATA_ARR_SIZE  56 //784

#define IRCAM_TX_DATA_CHUNK_SIZE 28
#define IRCAM_DATA_ARR_SIZE 784

//---------------------------- Define all objects ---------------------------------
SoftwareSerial WindSerial(WIND_RX_PIN, WIND_TX_PIN); // RX=16 , TX =17
//SoftwareSerial GpsSerial(GPS_RX_PIN, GPS_TX_PIN); // The serial connection to the GPS device
TinyGPSPlus GPS;
Adafruit_AHTX0 AHT;
MS5611 Ms5611;
paramsMLX90640 MLX90640;
//RF24 radio(TELE_CE_PIN, TELE_CSN_PIN); // Create a Radio || using pin 14 for the CE pin, and pin 12 for the CSN pin
//RF24Network network(radio); 
DS3231 Clock;
//SoftwareSerial pxSerial(33,32); // RX, TX
SerialTransfer IrCamSerialTransfer;
SoftwareSerial telemetrySerial(35,14);


//TinyGPSCustom pdop(GPS, "GNGLL", 1); // $GPGSA sentence, 15th element
//TinyGPSCustom hdop(GPS, "GNGLL", 3); // $GPGSA sentence, 16th element

// --------------------------- Define global vars ---------------------------------

// Wind sensor vars -------------------------------------------
String  wind_query = "$//,WV?*//\r\n" ;                                             
char    wind_inp_buffer[64];
char    wind_inp_buffer_temp[64];
char    wind_msg_term_char = '\n';
char    wind_char = '\0';
int     wind_indx = 0;
float   wind_speed = 0.0;
float   wind_angle = 0.0;
boolean wind_have_new_data = false;
double  wind_acoustic_temp;
float   temperature_acoustic_wind_sensor = 0.0;
char    temp_reading_validity;
char    wind_sensor_status;

// Humidity temperature sensor --------------------------------
sensors_event_t humidity_aht10, temp_aht10;
float temperature_aht10 = 0.0;
float relative_humidity_aht10 = 0.0;

// Pressure temperature sensor --------------------------------
double reference_pressure = 0.0;
double temperature_ms5611 = 0.0;
long   pressure_ms5611 = 0;
float  absolute_altitude_ms5611 = 0.0;

// Gps --------------------------------------------------------
static const uint32_t gps_baud_rate = 9600;
double latitude = 0.0;
double longitude = 0.0;
double altitude_gps = 0.0;

// IR Camera --------------------------------------------------
const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640
static float mlx90640To[768] = {0};

// SD card ----------------------------------------------------
  
typedef struct {      // Type for system initialization variables
  uint16_t bootCount; // Arduino boot counter (incremented in every boot)
  uint16_t fileName;  // Variable for unique data file name on SD card
} sysinit_t; 

sysinit_t sysinit;    // Variable that holds system initialization data
char* filename;
File SensorDataFile;

unsigned long currentMillis_SDcard=0;
unsigned long prevMillis_SDcard=0; 
unsigned long SDcard_reopen_interval = 10000;

// Telemetry -------------------------------------------------
const uint16_t ADDRESS_THIS_NODE = 01;       // Address of this in Octal format
const uint16_t ADDRESS_BASE_NODE = 00;      // Address of base station node in Octal format

struct payload_t {                   // Structure of our payload
  unsigned long msgID;
  unsigned long startIndx;
  unsigned long dataLen;
  float sensorData[TX_DATA_CHUNK_SIZE];
};

int noOfTransReTries = 2;

// timer vars-------------------------------------------------
int Year;
int Month;
int Date;
int Hour;
int Minute;
int Second;
int MilliSecond;

long initTime;
int initMillis;

bool century = false;
bool h12Flag;
bool pmFlag;

// Pixhawk data
long time_boot_ms_attitude;
float roll;
float pitch;
float yaw;
float rollspeed;
float pitchspeed;
float yawspeed;

long time_boot_ms_pix_gps;
long pix_lat;
long pix_lon;
long pix_alt;
long pix_relative_alt;
long pix_gps_vx;
long pix_gps_vy;
long pix_gps_vz;

long time_boot_ms_loc_pos;
float x;
float y;
float z;
float vx;
float vy;
float vz;

// -------------------- IR cam comms ------------------------------------

struct ircam_payload_t {                   // Structure of our payload
  unsigned long msgID;
  unsigned long startIndx;
  unsigned long dataLen;
  float sensorData[IRCAM_TX_DATA_CHUNK_SIZE];
};
float IrCamDataArray[IRCAM_DATA_ARR_SIZE];
bool NewDataRcvd = false;


// Misc vars -------------------------------------------------
unsigned long uptime;
float sensorDataArray[DATA_ARR_SIZE];
//memset(sensorDataArray, 0.0, sizeof(sensorDataArray));
int loopID = 1;
String timeStampMemoryCard = "";

unsigned long currentMillis=0;
unsigned long prevMillis=0; 
unsigned long wind_sensor_reading_interval = 200;

TaskHandle_t Task1;
//TaskHandle_t Task2;
  
void setup() {
  Serial.begin(57600);  
  pxSerial.begin(57600);
//  pxSerial.begin(57600,SERIAL_8N1,33,32);
  Mav_Request_Data();
  delay(50);
  Wire.begin();
  Wire.setClock(400000); //Increase I2C clock speed to 400kHz
  WindSerial.begin(9600);      
  telemetrySerial.begin(57600);
//  telemetrySerial.begin(57600,SERIAL_8N1,12,14);
  
  
  
  Serial2.begin(57600, SERIAL_8N1, IRCAM_RX_PIN, IRCAM_TX_PIN);
  IrCamSerialTransfer.begin(Serial2);

  updateInitTime();
//  
//  
//  IRCamInitConfig();   // Intialise the configurations of the IR camera
  InitialiseHumidityTempSensor();
  InitialisePressureSensor();  
  IntialiseSDCard();   
//  IntialiseTelemetry();
  

  pinMode(WIND_RCV_ENABLE_PIN, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(WIND_RCV_ENABLE_PIN, HIGH);


//xTaskCreate(
  xTaskCreatePinnedToCore(
             Task1code, /* Task function. */
             "Task1",   /* name of task. */
             10000,     /* Stack size of task */
             NULL,      /* parameter of the task */
             0,         /* priority of the task */
             &Task1,   /* Task handle to keep track of created task */ //);
             0); 

}

void loop() {
//  Serial.print("Task1 running on core ");
//  Serial.println(xPortGetCoreID());

//  UpdateGpsReading();
  UpdateWindSensorReading();
  UpdateHumTempSensorReading();
  UpdatePressTempSensorReading();
////  UpdateIRCameraReading();
  getTimeStamp();
//  Mav_Request_Data();
  pixhawk_comm_receive();
  PrintValues();
  WriteSensorReadingToMemCard();
  currentMillis_SDcard = millis();
  if ((currentMillis_SDcard-prevMillis_SDcard)>=SDcard_reopen_interval){
    prevMillis_SDcard = currentMillis_SDcard;
    Serial.println("-------------------- Reopening file----------------------------------");
    SensorDataFile.close();
    SensorDataFile = SD.open(filename, FILE_APPEND);
  }
  TransmitSensorReading();
//delay(100);
}


void Task1code( void * pvParameters ){
 
  for(;;){
    UpdateIRCamData();
  }
}
