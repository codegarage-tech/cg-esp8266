/*
 * Interfacing:
 * https://www.youtube.com/watch?v=BXbDAyR5cIU
 * https://medium.com/@cgrant/using-the-esp8266-wifi-module-with-arduino-uno-publishing-to-thingspeak-99fc77122e82
 *      
 *    PINOUT: 
 *        _______________________________
 *       |  ARDUINO UNO   >>>   ESP8266  |
 *        -------------------------------
 *        GND      >>>   GND
 *        3.3v     >>>   VCC
 *        3.3v     >>>   CH_PD
 *        RX(<-2)  >>>   TX
 *        TX(<-3)  >>>   RX
 *
 *    NOTE: ESP8266 runs with 3.3v
 *
 *        Created on: Jan 10, 2019
 *        Author: Md. Rashadul Alam
 *        Email: rashed.droid@gmail.com
 */

#include <SoftwareSerial.h>
SoftwareSerial ESPserial (2,3);

void setup() {
  Serial.begin(115200);
  ESPserial.begin(115200);
}

void loop() {
    if (ESPserial.available()){
      Serial.write(ESPserial.read());
    }

    if (Serial.available()){
      ESPserial.write(Serial.read());
    }
}
