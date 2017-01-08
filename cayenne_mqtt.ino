/**************************************
* Meteo Station
* Arduino UNO + Ethrenet Shield + Cayenne MQTT
* by Giovanni Gentile
* http://www.0lab.it
* Creative Common License
**************************************/
#include <Wire.h>
#include <Adafruit_BMP085.h>

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTEthernet.h>

// Count
unsigned long lastMillis = 0;

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "USER";
char password[] = "PASSWORD";
char clientID[] = "PASS";

//Temperatura interno
int tempPin = 0;   // Analogic PIN
float temp; // temp val
float volt; 

//Foto interno
int photoPin = A1; // pin IN photoresistor
int lightin;

//Foto esterno
int photoPinout = A2; // pin IN photoresistor
int lightout;

/*Barometro*
Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
Connect GND to Ground
Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
EOC is not used, it signifies an end of conversion
XCLR is a reset pin, also not used here
*/

Adafruit_BMP085 bmp;
int outTemp;
int outBaro;

void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID);
  bmp.begin();
}

void loop() {
  Cayenne.loop();

  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    
    //temp = ((5 * analogRead(tempPin) * 100.0 ) / 1024) - 55; // Temp indor
    temp = 33;
    
    // Lux indor
    lightin = analogRead(photoPin);
    lightin = map(lightin, 0, 1023, 0, 110);
    
    outTemp = bmp.readTemperature(); // Temp ext
    delay(20);
    //outBaro = bmp.readPressure(); // Press ext
    outBaro=1064;
    
    // Lux ext
    //lightout = analogRead(photoPinout);
    lightout = map(lightout, 0, 1023, 0, 110); 
    
    // Push
    Cayenne.virtualWrite(0, lastMillis/1000); // Count
    Cayenne.celsiusWrite(1, temp);            // Temp indor
    Cayenne.luxWrite(2, lightin);             // Lux indor
    Cayenne.celsiusWrite(3, outTemp);         // Temp out
    Cayenne.virtualWrite(4, outBaro/100);     // Baro out
    Cayenne.luxWrite(5, lightout);            // Lux out
  }
}

CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
}
