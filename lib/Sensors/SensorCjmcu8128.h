#ifndef SensorCjmcu8128_h_
#define SensorCjmcu8128_h_

#include <Arduino.h>
#include <SensorBase.h>
#include <Preferences.h>
#include <Constants.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SensorCjmcu8128Root.h>
#include <SensorCjmcu8128Settings.h>

class SensorCjmcu8128 : public SensorBase
{
    public:
        void begin();
        void executeReporting(String baseAddress);
        String getConfigurationPageHtml();
        String getSensorInformationHtml();
        String getNamePrefix();
        String getSettings();
        String getValues();
        void loop();
        void resetSettings();
        void updateSettings(String settings);
    
    private:
        const char* SENSOR_CJMCU8128_PREF_SDA_PIN = "sdaPin";
        const char* SENSOR_CJMCU8128_PREF_SCL_PIN = "sclPin";
        const char* SENSOR_CJMCU8128_PREF_SCAN_DEVICES = "scanDevs";

        void logMessage(String msg);
        bool readSettings();
        void runDeviceScan();

        int settingSdaPin;
        int settingSclPin;
        bool settingScanDevices;

        bool validSettings = false;
};

#endif