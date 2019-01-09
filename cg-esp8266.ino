// PHYSICAL STUFF:
// Arduino pin 2 (RX) to ESP8266 TX
// Arduino pin 3 (RX) to Voltage Divider, then to ESP8266 RX
// Connect GND from the Arduino to GMD on the ESP8266
// Connect Arduino 3.3V to VCC
// Pull ESP8266 CH_PD HIGH (also connect to 3.3V rail)

// Commands entered in the serial monitor are relayed to the ESP8266

#include <SoftwareSerial.h>
SoftwareSerial ESPserial (2,3); //RX|TX


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  ESPserial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (ESPserial.available()){
    Serial.write(ESPserial.read());
    }

    if (Serial.available()){
    ESPserial.write(Serial.read());
    }
}
