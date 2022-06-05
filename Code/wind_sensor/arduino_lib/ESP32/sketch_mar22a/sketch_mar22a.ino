//Master code (Arduino UNO)
//Serial Communication Between Arduino using RS-485
#include <SoftwareSerial.h>

SoftwareSerial mod(16,17); // RX=16 , TX =17

#define RXD1 19
#define TXD1 21
int enablePin1 = 4;
//int enablePin2 = 2;
//String id_query = "$//,ID?*//\r\n";
String id_query = "$//,WV?*//\r\n";
char inp_buffer[64];
char term_char = '\n';
char c;
int ind = 0;
boolean haveNewData = false;

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

  digitalWrite(enablePin1, HIGH);
//  digitalWrite(enablePin2, HIGH);
  mod.print(id_query);
//  Serial1.print(id_query);
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
    Serial.print(c);
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
  Serial.println(inp_buffer);
  digitalWrite(LED_BUILTIN, LOW);
  haveNewData = false;

  //    delay(500);


}
