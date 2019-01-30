// This is a very simple httpUpdate sample sketch based on the example that comes with the ESP8266 Arduino package
// Once uploaded to ESP8266, this sketch will blink the built-in LED at 2 seconds interval and download new firmware that blink the built-in LED at 1 second interval via OTA after 25 loops.
//http://wei48221.blogspot.com/2017/10/esp8266-ota-over-internet-using-http.html
//https://www.bakke.online/index.php/2017/06/02/self-updating-ota-firmware-for-esp8266/
//https://circuits4you.com/2017/12/31/software-reset-esp8266/
//https://techtutorialsx.com/2017/12/29/esp8266-arduino-software-restart/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

ESP8266WiFiMulti WiFiMulti;

int ledState = LOW;

unsigned long previousMillis = 0;
const long interval = 2000;

int count = 0;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
}

void ota_update() {
    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFiMulti.addAP("Dicosta", "ramo011911");

    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        t_httpUpdate_return ret = ESPhttpUpdate.update("http://iot-cuddle.000webhostapp.com/iot/fota/ota-test.ino.generic.bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                break;
        }
    }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
     previousMillis = currentMillis;

     if (ledState == LOW)
         ledState = HIGH;  // Note that this switches the LED *off*
     else
         ledState = LOW;   // Note that this switches the LED *on*

    digitalWrite(LED_BUILTIN, ledState);
    Serial.println("Blink LED at 2 seconds interval");

    count = count + 1;
    if (count > 25) ota_update();
  }
}
