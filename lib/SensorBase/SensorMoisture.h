#ifndef SensorMoisture_h_
#define SensorMoisture_h_

#include <SensorBase.h>

class SensorMoisture : public SensorBase
{
    public:
        void resetSettings();
        void begin();
        String getSensorInformationHtml();
};

#endif