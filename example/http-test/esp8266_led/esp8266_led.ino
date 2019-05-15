#include <RGBLED.h>

#include <ezTime.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
//#define COMMON_ANODE
#define LOCALTZ_POSIX  "CET-1CEST,M3.4.0/2,M10.4.0/3"    // Time in Berlin

Timezone local;
Timezone pacific;

const char* ssid     = "Dicosta";
const char* password = "ramo011911";
//const char* host = "iot-cuddle.000webhostapp.com";
//int gpio = 1;
int GPIO0 = 0, GPIO2 = 2, TX = 1, RX = 3;

// Declare an RGBLED instanced named rgbLed
// Red, Green and Blue LED legs are connected to PWM pins 11,9 & 6 respectively
// In this example, we have a COMMON_ANODE LED, use COMMON_CATHODE otherwise
//RGBLED rgbLed(TX, GPIO2, RX, COMMON_ANODE);

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
  pinMode(GPIO0, OUTPUT);
  //////////////////////
  pinMode(TX, OUTPUT);
  pinMode(GPIO2, OUTPUT);
  pinMode(RX, OUTPUT);
  ///////////////////////
  digitalWrite(GPIO0, HIGH);
  ///////////////////
  analogWrite(TX, 1023);
  analogWrite(GPIO2, 1023);
  analogWrite(RX, 1023);
  /////////////////////////
  Serial.println();

  //   Connect to wifi
  connectToWifi();
}

// the loop function runs over and over again forever
void loopBlink() {
  delay(5000);
  digitalWrite(GPIO0, LOW);
  Serial.print("GPIO0 pin low\n");
  //  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(5000);                      // Wait for a second
  digitalWrite(GPIO0, HIGH);
  Serial.print("GPIO0 pin high\n");
  //  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  //  delay(1000);                      // Wait for two seconds (to demonstrate the active low LED)
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
            digitalWrite(GPIO0, LOW);
            delay(100);
            Serial.println("GPIO 0 is On..!");
          } else if (gpioStatus == "off") {
            digitalWrite(GPIO0, HIGH);
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
  //////////////////////////////////////////
  //  //Control wifi led signal
  //   long rssi = WiFi.RSSI();
  //   Serial.println("rssi: " + rssi);
  //   int bars = getBarsSignal(rssi);
  //   Serial.println("bars: " + bars);
  //   setWifiSignlaLed(bars);

  //static int previousQuality = -1;
  //  int quality = getQuality();
  //  if (quality != previousQuality) {  // If the quality changed since last print, print new quality and RSSI
  //    if (quality != -1){
  //      Serial.printf("WiFi Quality:\t%d\%\tRSSI:\t%d dBm\r\n", quality, WiFi.RSSI());
  int bars = getBarsSignal(WiFi.RSSI());
  //      Serial.printf("bars: \t%d\r\n" , bars);
  if ((bars == 4) || (bars == 5)) {
    Serial.println("Wifi Signal is good: 4 or 5");
    //Set the RGBLED to show GREEN only
    //    rgbLed.writeRGB(0, 255, 0);
    analogWrite(GPIO2, 255);
    analogWrite(RX, 1023);
    analogWrite(TX, 1023);
  } else if (bars == 3) {
    Serial.println("Wifi Signal is medium: 3");
    //Set the RGBLED to show BLUE only
    //    rgbLed.writeRGB(0, 0, 255);
    analogWrite(GPIO2, 1023);
    analogWrite(RX, 1023);
    analogWrite(TX, 255);
  } else if ((bars == 1) || (bars == 2)) {
    Serial.println("Wifi Signal is low: 1 or 2");
    //Set the RGBLED to show YELLOW (RED & GREEN) only
    //    rgbLed.writeRGB(255, 255, 0);
    analogWrite(GPIO2, 1023);
    analogWrite(RX, 255);
    analogWrite(TX, 1023);
  } else if (bars == 0) {
    Serial.println("Wifi Signal is not available: 0");
    //Set the RGBLED to show RED only
    //    rgbLed.writeRGB(255, 0, 0);
    analogWrite(GPIO2, 1023);
    analogWrite(RX, 1023);
    analogWrite(TX, 1023);
  }
  //    }
  //    previousQuality = quality;
  //  }

  //////////////////////////////////////////
  Serial.println("Wait 3s before next round...");
  delay(3000);
}

//void setColor(int red, int green, int blue)
//{
//  #ifdef COMMON_ANODE
//    red = 255 - red;
//    green = 255 - green;
//    blue = 255 - blue;
//  #endif
//  analogWrite(redPin, red);
//  analogWrite(greenPin, green);
//  analogWrite(bluePin, blue);
//}

int getBarsSignal(long rssi) {
  // 5. High quality: 90% ~= -55db
  // 4. Good quality: 75% ~= -65db
  // 3. Medium quality: 50% ~= -75db
  // 2. Low quality: 30% ~= -85db
  // 1. Unusable quality: 8% ~= -96db
  // 0. No signal

  int bars = 0;
  if (WiFi.status() == WL_CONNECTED) {
    if (rssi > -55) {
      bars = 5;
    } else if (rssi < -55 & rssi > -65) {
      bars = 4;
    } else if (rssi < -65 & rssi > -75) {
      bars = 3;
    } else if (rssi < -75 & rssi > -85) {
      bars = 2;
    } else if (rssi < -85 & rssi > -96) {
      bars = 1;
    } else {
      bars = 0;
    }
  }

  return bars;
}

///*
//   Return the quality (Received Signal Strength Indicator)
//   of the WiFi network.
//   Returns a number between 0 and 100 if WiFi is connected.
//   Returns -1 if WiFi is disconnected.
//*/
//int getQuality() {
//  if (WiFi.status() != WL_CONNECTED)
//    return -1;
//  int dBm = WiFi.RSSI();
//  if (dBm <= -100)
//    return 0;
//  if (dBm >= -50)
//    return 100;
//  return 2 * (dBm + 100);
//}

//void fade(int pin) {
//  for (int u = 0; u < 1024; u++) {
//    analogWrite(pin, 1023 - u);
//    delay(1);
//  }
//  for (int u = 0; u < 1024; u++) {
//    analogWrite(pin, u);
//    delay(1);
//  }
//}
