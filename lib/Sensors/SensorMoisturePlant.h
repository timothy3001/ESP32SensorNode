#ifndef SensorMoisturePlant_h_
#define SensorMoisturePlant_h_


#include <Arduino.h>
#include <ArduinoJson.h>
#include <SensorBase.h>
#include <Constants.h>
#include <Preferences.h>
#include <HelperFunctions.h>
#include <SensorPlantMoistureRoot.h>
#include <SensorPlantMoistureSettings.h>

class SensorMoisturePlant : public SensorBase
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
        const char* SENSOR_MOISTURE_PLANT_PREF_DATA_PIN = "dataPin";
        const char* SENSOR_MOISTURE_PLANT_PREF_REPORTING_ADDRESS = "pmPercSubAddr";
        const char* SENSOR_MOISTURE_PLANT_PREF_MAX_VALUE = "pmMaxVal";
        const char* SENSOR_MOISTURE_PLANT_PREF_MIN_VALUE = "pmMinVal";

        int settingDataPinSensor;
        String settingAddressValue;
        int settingMaxValue;
        int settingMinValue;

        float currentMoisturePercentage = -1.0f;
        int currentMoistureAbsolute = -1;

        bool validSettings = false;
        unsigned long lastUpdate = 0;

        void logMessage(String msg);
        bool readSettings();
        void updateSensorValue();
        bool isSensorValueValid();

};

#endif