#include "utils.h"

boolean initConfig(ConfigStore &configStore)
{
    EEPROM.begin(sizeof(ConfigStore));
    return readConfig(configStore);
}

// 将配置写入flash
void writeConfig(ConfigStore &configStore) { 
    EEPROM.put(EEPROM_CONFIG_START, configStore);
    EEPROM.commit();
}

// 从flash读取配置
boolean readConfig(ConfigStore &configStore) { 
    EEPROM.get(EEPROM_CONFIG_START, configStore);
    if (configStore.magic != configDefault.magic) {
        DEBUG_PRINT("Using default config.");
        configStore = configDefault;
        return false;
    }
    else
        return true;
}

void connectWiFi(ConfigStore &configStore)
{
    int try_count = 0;
    boolean connected = true;
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(configStore.wifiSSID, configStore.wifiPass);
        pinMode(LED_PIN, OUTPUT);
        DEBUG_PRINT("Connecting to WiFi.");
        while(WiFi.status() != WL_CONNECTED)
        {
            digitalWrite(LED_PIN, HIGH);
            delay(250);
            digitalWrite(LED_PIN, LOW);
            delay(250);
            try_count++;
            if(try_count > 20)
            {
                connected = false;
                break;
            }
        }
        if(!connected)
        {
            DEBUG_PRINT("Connect to WiFi Failed.");
            smartConfig(configStore);
        }
    }
}


void monitorWiFi(ConfigStore &configStore)
{
    if(configStore.flagConfig == 1)
    {
        WiFi.disconnect();
        configStore.flagWiFiFail = 1;
        smartConfig(configStore);
    }

    if (WiFi.status() != WL_CONNECTED) {
        if(configStore.flagWiFiFail == 0)
            configStore.flagWiFiFail = 1;
        connectWiFi(configStore);
    }
    else if (configStore.flagWiFiFail == 1)
    {
        configStore.flagWiFiFail = 0;
        DEBUG_PRINT("WiFi connected:");
        DEBUG_PRINT(WiFi.SSID().c_str());
        DEBUG_PRINT("Local IP:");
        DEBUG_PRINT(WiFi.localIP());
    }
    else
        configStore.flagWiFiFail == 0;
}

// 通过Smart Config获取密码
void smartConfig(ConfigStore &configStore)
{
    boolean flag = true;
    uint8_t brightness;//, m_Counter = 0;
    uint16_t m_Counter = 0;
    uint8_t i = 0;
    WiFi.mode(WIFI_STA);
    DEBUG_PRINT("Wait for SmartConfig");
    WiFi.beginSmartConfig();
    pinMode(LED_PIN, OUTPUT);
    while (flag) {
        if(m_Counter == 0) Serial.print(".");
        // analogWriteRange(new_range)
        // analogWriteFreq(new_frequency) 
        //analogWriteResolution(10);
        //analogWrite(LED_PIN, map(brightness, 0, 1023, 0, 1023));
        m_Counter = (m_Counter + 1) % 256;
        brightness = (m_Counter < 128) ? m_Counter : 256 - m_Counter;
        // m_Counter = (m_Counter + 1) % 1024;
        // brightness = (m_Counter < 512) ? m_Counter : 1024 - m_Counter;
        //analogWrite(LED_PIN, BOARD_PWM_MAX * ((float)brightness / (BOARD_PWM_MAX/2)));
        analogWrite(LED_PIN, BOARD_PWM_MAX - brightness * 2);
        delay(10);
        
        if (WiFi.smartConfigDone()) {
            Serial.print("");
            DEBUG_PRINTF("\r\nSmartConfig Success.");
            // DEBUG_PRINTF("SSID: %s\r\n", WiFi.SSID().c_str());
            // DEBUG_PRINTF("PSW: %s\r\n", WiFi.psk().c_str());

            // 保存配置
            DEBUG_PRINT("Save config...");
            // sprintf(configStore.wifiSSID, "%s", WiFi.SSID().c_str());
            // sprintf(configStore.wifiPass, "%s", WiFi.psk().c_str());
            memset(configStore.wifiSSID, 0, sizeof(configStore.wifiSSID));
            memset(configStore.wifiPass, 0, sizeof(configStore.wifiPass));
            strcpy(configStore.wifiSSID, WiFi.SSID().c_str());
            strcpy(configStore.wifiPass, WiFi.psk().c_str());
            DEBUG_PRINTF("SSID: %s\n", configStore.wifiSSID);
            DEBUG_PRINTF("PSW: %s\n", configStore.wifiPass);
            writeConfig(configStore);
            DEBUG_PRINT("Save config done!");
            WiFi.stopSmartConfig();
            flag = false;
        }
    }
    for(i=0; i<10; i++) // 状态提示
    {
        delay(100);
        digitalWrite(LED_PIN, i % 2);
    }
}



// 固件升级
void firmwareUpdate(ConfigStore &configStore)
{
    DEBUG_PRINTF("Updating firmware -> ");
    //t_httpUpdate_return ret = ESPhttpUpdate.update(configStore.cloudHost, configStore.cloudPort, configStore.cloudUpdateUrl, configStore.version);
    //t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.1.101:5000/firmware", configStore.version);
    //t_httpUpdate_return ret = ESPhttpUpdate.update("https://server/file.bin");
      t_httpUpdate_return ret = ESPhttpUpdate.update("http://iot-cuddle.000webhostapp.com/iot/fota/ota-test.ino.generic.bin");
    
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            DEBUG_PRINT("HTTP_UPDATE_FAILED");
            break;

        case HTTP_UPDATE_NO_UPDATES:
            DEBUG_PRINT("HTTP_UPDATE_NO_UPDATES");
            break;

        case HTTP_UPDATE_OK:
            DEBUG_PRINT("HTTP_UPDATE_OK");
            break;
    }
}
// 检查固件升级信息
int checkFirmwareUpdate(ConfigStore &configStore)
{
    int result = 0;
    HTTPClient http;
    http.begin(configStore.cloudHost, configStore.cloudPort, configStore.cloudFwCheckUrl);
    int httpCode =  http.GET(); // 使用GET形式来取得数据
    DEBUG_PRINTF("Code: %d\n", httpCode);
    if(httpCode == 200) { // 访问成功，取得返回参数
        String payload = http.getString();
        DEBUG_PRINTF("Context: %s\n", payload.c_str());
        if(payload == "1")
            result = 1;
        else
            result = 0;
    } else { // 访问不成功，打印原因
        String payload = http.getString();
        DEBUG_PRINTF("Context: %s\n", payload.c_str());
        result = 0;
    }

    //Now return 1 is fixed for default, for getting always firmware update
    //We need to get this value from server
    return 1;
}
