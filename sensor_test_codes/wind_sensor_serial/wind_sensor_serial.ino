
#include <SoftwareSerial.h>

SoftwareSerial mod(16,17); // RX=16 , TX =17

#define RXD1 19
#define TXD1 21
int enablePin1 = 4;
//int enablePin2 = 2;
//String query = "$//,ID?*//\r\n";
String query2 = "$//,WV?*//";
String query = "$//,WV?*//";
//String query2 = "test";
//String query = "test";
char inp_buffer[64];
char term_char = '\n';
char c;
int ind = 0;
boolean haveNewData = false;

char msg_first_part[32];
char msg_value_part[32];
char msg_temp_holder[32];

float wind_speed = 0.000;
float wind_angle = 0.000;
char Status ;
float Temp = 0.0;
char Validity ;

void setup()
{
  Serial.begin(9600);            // initialize serial at baudrate 9600:
  //  Serial1.begin(9600);
//  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
  mod.begin(9600);
//  Serial2.begin(9600);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for Native USB only
  //  }
  pinMode(enablePin1, OUTPUT); 
//  pinMode(enablePin2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(10);

}
void loop()
{
  while (Serial.available() == 0){}
  query = Serial.readStringUntil('\n');
  Serial.print("Recieved command : ");
  Serial.println(query);
  query.trim();
  query  = query + "\r\n";
//  query = "$01,AT?*//\r\n";
  digitalWrite(enablePin1, HIGH);
//  digitalWrite(enablePin2, HIGH);
  mod.print(query);
//  Serial1.print(query);
  digitalWrite(enablePin1, LOW);
//  digitalWrite(enablePin2, LOW);
  delay(200);
  


  while (mod.available() && !haveNewData) {
//    while (Serial1.available() && !haveNewData) {

    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//    delay(100);                       // wait for a secondconver
//    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//    delay(100);                       // wait for a second

    c = mod.read();
//    c = Serial1.read();
//    Serial.print(c);
    if (c != term_char)
    {
      inp_buffer[ind] = c;
      ind = ind + 1;
    }
    else
    {
      inp_buffer[ind] = '\0';
      ind = 0;
      haveNewData = true;
    }

  }
  //    Serial.print(Serial1.read());
  ind=0;
//  strncpy(inp_buffer, "$WI,WVP=001.8,045,0*73\r\n", 64);
//
  Serial.println(inp_buffer);
  digitalWrite(LED_BUILTIN, LOW);
//  parseData();
//  
//
  haveNewData = false;
//  memset(inp_buffer, 0, sizeof inp_buffer);

      delay(100);


}

void parseData(){
 char * strtokIndx;
 strtokIndx = strtok(inp_buffer,",="); 
 strtokIndx = strtok(NULL,",=");
 
 strtokIndx = strtok(NULL,",=");
 wind_speed = atof(strtokIndx);
 
 strtokIndx = strtok(NULL,",=");
 wind_angle = atof(strtokIndx);

 strtokIndx = strtok(NULL,",=");
 Status = *strtokIndx;

 strtokIndx = strtok(NULL,",=");
 Temp = atof(strtokIndx);

 strtokIndx = strtok(NULL,",=");
 strtokIndx = strtok(NULL,",=*");

 Validity = *strtokIndx;
 Serial.println(Validity);

 Serial.println(Status);
 
 Serial.print("wind speed: ");
 Serial.println(wind_speed);
 Serial.print("Wind angle: ");
 Serial.println(wind_angle);
 Serial.print("Temperature: ");
 Serial.println(Temp);
 
}
