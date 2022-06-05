#include <SoftwareSerial.h>
SoftwareSerial ArduinoSlave(26,25);
String answer;
String msg;
int intVal=0,oldIntVal=0;
void setup(){
Serial.begin(9600);
Serial.println("ENTER Commands:");
ArduinoSlave.begin(9600);
}
void loop(){
//Read sensor
intVal=20;
//Read answer from slave
readSlavePort();
//Send data to slave
if(oldIntVal!=intVal){
Serial.print("Master sent : ");
Serial.println(intVal);
ArduinoSlave.print(intVal);
oldIntVal=intVal;
}
//Send answer to monitor
if(answer!=""){
Serial.print("Slave LED PWM value : ");
Serial.println(answer);
answer="";
}
delay(1000);
}
void readSlavePort(){
while (ArduinoSlave.available()) {
delay(10);
if (ArduinoSlave.available() >0) {
char c = ArduinoSlave.read();  //gets one byte from serial buffer
answer += c; //makes the string readString
}
}
}
