#include <Arduino.h>
#include <SensorBase.h>

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
        const char* SENSOR_MOISTURE_PLANT_PREF_DATA_PIN = "dataPin";
        const char* SENSOR_MOISTURE_PLANT_PREF_REPORTING_ADDRESS = "moistAdd";

        int settingDataPinSensor;
        String settingAddressValue;

        float currentMoisture = -1.0f;
        bool validSettings = false;
        unsigned long lastUpdate = 0;

        void logMessage(String msg);

};