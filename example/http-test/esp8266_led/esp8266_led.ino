#include <ezTime.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define LOCALTZ_POSIX  "CET-1CEST,M3.4.0/2,M10.4.0/3"    // Time in Berlin

Timezone local;
Timezone pacific;

const char* ssid     = "Dicosta";
const char* password = "ramo011911";
//const char* host = "iot-cuddle.000webhostapp.com";
//int gpio = 1;

void connectToWifi() {
  delay(1000);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

void setup() {
  // Setup serial port
  Serial.begin(115200);
  delay(10000);
  while (!Serial) {    // wait for Serial port to connect. Needed for native USB port only
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Dealing with current date-time
  local.setPosix(LOCALTZ_POSIX);
  local.setTime(compileTime());
  Serial.print(F("Local time   :  "));
  Serial.println(local.dateTime());
  pacific.setPosix(F("PST+8PDT,M3.2.0/2,M11.1.0/2"));
  Serial.print(F("Pacific time :  "));
  Serial.println(pacific.dateTime());
  Serial.print(F("UTC          :  "));
  Serial.println(UTC.dateTime());

  // Setup different pins
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
  Serial.println();

  // Connect to wifi
  connectToWifi();
} 

// the loop function runs over and over again forever
void loopBlink() {
  digitalWrite(0, LOW);
//  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(0, HIGH);
//  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(3000);                      // Wait for two seconds (to demonstrate the active low LED)
}

void loop() {
  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = "";
//    if (gpio == 1) {
      Serial.println("Preparing GPIO 0 url");
      url = "http://iot-cuddle.000webhostapp.com/iot/api/led/specific.php?id=1";
//    } else if (gpio == 2) {
//      Serial.println("Preparing GPIO 2 url");
//      url = "https://iot-cuddle.000webhostapp.com/iot/api/led/specific.php?id=2";
//    }
    Serial.print("Requesting URL: ");
    Serial.println(url);

    Serial.print("[HTTP] begin...\n");
    http.begin( url);

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        Serial.printf("[HTTP] GET... success");
        String payload = http.getString();

        if (payload != NULL && payload.length() > 0) {
          Serial.println("response: " + payload);

          // Parse JSON
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& json_parsed = jsonBuffer.parseObject(payload);
          if (!json_parsed.success()) {
            Serial.println("parseObject() failed");
            return;
          }
          String gpioId = json_parsed["led"][0]["id"];
          String gpioStatus = json_parsed["led"][0]["status"];
          Serial.println("gpioId: " + gpioId);
          Serial.println("gpioStatus: " + gpioStatus);

          //do further task
//          if (gpioId == "1") {
            if (gpioStatus == "on") {
              digitalWrite(0, LOW);
              delay(100);
              Serial.println("GPIO 0 is On..!");
            } else if (gpioStatus == "off") {
              digitalWrite(0, HIGH);
              delay(100);
              Serial.println("GPIO 0 is Off..!");
            }
//            gpio = 2;
//          } else if (gpioId == "2") {
//            if (gpioStatus == "on") {
//              digitalWrite(2, LOW);
//              Serial.println("GPIO 2 is On..!");
//            } else if (gpioStatus == "off") {
//              digitalWrite(2, HIGH);
//              Serial.println("GPIO 2 is Off..!");
//            }
//            gpio = 1;
//          }
//          Serial.println("Next gpio id: " + gpio);
        } else {
          Serial.printf("[HTTP] GET... got null or empty response");
        }
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  Serial.println("Wait 3s before next round...");
  delay(3000);
}
