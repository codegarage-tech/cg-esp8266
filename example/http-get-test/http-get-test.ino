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

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Dicosta";
const char* password = "ramo011911";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting...");
  }
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/users/1");
    int httpCode = http.GET();

    if (httpCode > 0) 
    {
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
 
      int id = root["id"]; 
      const char* name = root["name"]; 
      const char* username = root["username"]; 
      const char* email = root["email"]; 

      Serial.print("Name:");
      Serial.println(name);
      Serial.print("Username:");
      Serial.println(username);
      Serial.print("Email:");
      Serial.println(email);
    }
    http.end(); //Close connection
  }
  delay(60000);
}
