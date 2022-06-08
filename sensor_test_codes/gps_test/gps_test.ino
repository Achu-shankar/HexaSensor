#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;  //declare gps object
SoftwareSerial sc(26,25); //RX pin 3 Tx pin 4
void setup() {
Serial.begin(9600);
sc.begin(9600);
Serial.println("GPS started");
}

void loop()
{
Serial.write(sc.read());
//while (sc.available()){
//    gps.encode(sc.read());
//}
// Serial.print("lat: "); Serial.println(gps.location.lat(), 6);
// Serial.print("sat: "); Serial.println(gps.satellites.value());

}
