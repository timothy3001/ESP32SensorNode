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
        void logMessage(String msg);

};