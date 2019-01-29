#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid     = "Dicosta";
const char* password = "ramo011911";
const char* host = "iot-cuddle.000webhostapp.com";

String url;
int count = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
//  pinMode(D3, OUTPUT);
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
//  digitalWrite(D3, LOW);
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
  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(0, LOW);
  delay(500);
}

void loop() {

  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  if (count == 0){
    url = "/iot/api/led/specific.php?id=1";
    count = count + 1;
    Serial.println("Here1");
  } else if(count == 1){
    url = "/iot/api/led/specific.php?id=2";
    count = count + 1;
    Serial.println("Here2");
  }
//  else if(count == 2){
//    url = "/iot/api/led/specific.php?id=3";
//    count = count + 1;
//    Serial.println("Here3");
//  }
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    //Serial.print(line);
    // we’ll parse the HTML body here
    if (section=="header") { // headers..

      if (line=="\n") { // skips the empty space at the beginning
        section="json";
      }
    } else if (section=="json") {  // print the good stuff
      section="ignore";
      String result = line.substring(1);

      // Parse JSON
      int size = result.length() + 1;
      char json[size];
      result.toCharArray(json, size);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if (!json_parsed.success()) {
        Serial.println("parseObject() failed");
        return;
      }
      String led = json_parsed["led"][0]["status"];

      if(count == 1){
        if(led == "on"){
          digitalWrite(0, HIGH);
          delay(100);
          Serial.println("GPIO0 is On..!");
        } else if(led == "off"){
          digitalWrite(0, LOW);
          delay(100);
          Serial.println("GPIO0 is Off..!");
        }
      } else if(count == 2){
        if(led == "on"){
          digitalWrite(2, HIGH);
          Serial.println("GPIO2 is On..!");
        } else if(led == "off"){
          digitalWrite(2, LOW);
          Serial.println("GPIO2 is Off..!");
        }
      }
//      else if(count == 3){
//        if(led == "on"){
//          digitalWrite(D3, HIGH);
//          Serial.println("D3 is On..!");
//        } else if(led == "off"){
//          digitalWrite(D3, LOW);
//          Serial.println("D3 is Off..!");
//        }
//        count = 0;
//      }

      if (count == 2){
          count = 0;
      }
    }
  }
  Serial.println();
  Serial.println("closing connection");
  delay(3000);
}
