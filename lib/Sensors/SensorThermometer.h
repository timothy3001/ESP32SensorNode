#ifndef SensorThermometer_h_
#define SensorThermometer_h_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SensorBase.h>
#include <Constants.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Preferences.h>
#include <HelperFunctions.h>
#include <SensorThermometerRoot.h>
#include <SensorThermometerSettings.h>

class SensorThermometer : public SensorBase
{
    public:
        void begin();
        void resetSettings();
        String getSensorInformationHtml();
        String getConfigurationPageHtml();
        void updateSettings(String settings);
        String getValues();
        String getSettings();
        void executeReporting(String baseAddress);
        String getNamePrefix();
        void loop();

    private:
        const char* SENSOR_THERMOMETER_PREF_DATA_PIN = "dataPin";
        const char* SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS = "tempAdd";

        int settingDataPinSensor;
        String settingAddressValue;        

        float currentTemperature = -127.0f;
        bool validSettings = false;
        unsigned long lastUpdate = 0;

        OneWire *oneWire = NULL;
        DallasTemperature *tempSensor = NULL; 

        void updateTemperature();
        bool isTempValid(float temperature);
        bool readSettings();

        void logMessage(String msg);
};

#endif