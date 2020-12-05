#ifndef SensorThermometer_h_
#define SensorThermometer_h_

#include <SensorBase.h>

class SensorThermometer : public SensorBase
{
    public:
        void resetSettings();
        void begin();
        String getSensorInformationHtml();
};

#endif