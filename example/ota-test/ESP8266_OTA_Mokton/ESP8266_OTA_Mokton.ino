#include <Arduino.h>
#include "utils.h"

ConfigStore configStore;

void setup() {
    
    PRINT_PORT.begin(115200);
    // PRINT_PORT.setDebugOutput(true);

    PRINT_PORT.println();
    PRINT_PORT.println();
    PRINT_PORT.print("Firmware version: ");
    PRINT_PORT.println(configDefault.version);


    for(uint8_t t = 5; t > 0; t--) {
        PRINT_PORT.printf("[Starting up] WAIT %d...\n", t);
        PRINT_PORT.flush();
        delay(1000);
    }

    if(initConfig(configStore))
        connectWiFi(configStore);
    else
        smartConfig(configStore);

    if (WiFi.status() == WL_CONNECTED)
    {
        PRINT_PORT.print("Local IP: ");
        PRINT_PORT.println(WiFi.localIP());
    }
}

void loop() {
    // wait for WiFi connection
    monitorWiFi(configStore);
    if((configStore.flagWiFiFail == 0) && (millis() % 10000 == 0)) { // 每十秒尝试一次更新固件
        if(checkFirmwareUpdate(configStore))
        {
            firmwareUpdate(configStore);
        }
    }
}

