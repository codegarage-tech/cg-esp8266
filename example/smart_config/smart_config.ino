//https://github.com/techiesms/ESP8266-Smart-Config/blob/master/smart_config/smart_config.ino
//http://techiesms.com/iot-projects/now-no-need-enter-ssid-name-password-inside-code-esp8266-smart-config/
//https://www.youtube.com/watch?time_continue=38&v=5D6lkFGPPSw

#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  /* Set ESP8266 to WiFi Station mode */
  WiFi.mode(WIFI_STA);
  /* start SmartConfig */
  WiFi.beginSmartConfig();

  /* Wait for SmartConfig packet from mobile */
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("SmartConfig done.");

  /* Wait for WiFi to connect to AP */
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
}
