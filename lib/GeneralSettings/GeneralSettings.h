#ifndef GeneralSettings_h_
#define GeneralSettings_h_

#include <Arduino.h>
#include <Constants.h>
#include <Preferences.h>

class GeneralSettings
{
    public:
        void initialize();
        void save();
        void resetSettings();

        String baseAddress;
        String sensorName;
        bool reportingActive;
        int intervalSeconds;
        bool passiveOperation;
        bool reportingBatteryActive;
        String reportingBatteryAddress;
        SensorType sensorType;

    private:
        String getShortMac();
};

#endif