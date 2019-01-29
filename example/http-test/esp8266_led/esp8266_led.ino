#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid     = "Dicosta";
const char* password = "ramo011911";
const char* host = "iot-cuddle.000webhostapp.com";

int gpio = 1;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password); 
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

void loop() {
  WiFiClient client;
  const int httpPort = 80;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }
  String url = "";
  if (gpio == 1){
    Serial.println("Preparing GPIO 0 url");
    url = "/iot/api/led/specific.php?id=1";
  } else if(gpio == 2){
    Serial.println("Preparing GPIO 2 url");
    url = "/iot/api/led/specific.php?id=2";
  }
  Serial.print("Requesting URL: ");
  Serial.println("https://iot-cuddle.000webhostapp.com"+url);
  
  client.print(String("GET ") +
                url +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print("line: "+line);
    // we’ll parse the HTML body here
    if (section=="header") { // headers..
      if (line=="\n") { // skips the empty space at the beginning
        section="json";
      }
    } else if (section=="json") {  // print the good stuff
      section="ignore";
      // Parse JSON
      String result = line.substring(1);
      Serial.println("result: "+result);
      int size = result.length() + 1;
      Serial.println("size: "+size);
      char json[size];
      result.toCharArray(json, size);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if (!json_parsed.success()) {
        Serial.println("parseObject() failed");
        return;
      }
      String gpioId = json_parsed["led"][0]["id"];
      String gpioStatus = json_parsed["led"][0]["status"];
      if(gpioId== "1"){
        if(gpioStatus == "on"){
          digitalWrite(0, HIGH);
          delay(100);
          Serial.println("GPIO 0 is On..!");
        } else if(gpioStatus == "off"){
          digitalWrite(0, LOW);
          delay(100);
          Serial.println("GPIO 0 is Off..!");
        }
        gpio = 2;
      } else if(gpioId == "2"){
        if(gpioStatus == "on"){
          digitalWrite(2, HIGH);
          Serial.println("GPIO 2 is On..!");
        } else if(gpioStatus == "off"){
          digitalWrite(2, LOW);
          Serial.println("GPIO 2 is Off..!");
        }
        gpio = 1;
      }
      Serial.println("Next gpio id: "+ gpio);
      }
  }
  
  Serial.println("closing connection");
  client.stop();

  Serial.println("wait 5 sec...");
  delay(5000);
}
