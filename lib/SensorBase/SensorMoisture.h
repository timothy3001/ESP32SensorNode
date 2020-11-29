#include <SensorBase.h>

class SensorMoisture : public SensorBase
{
    public:
        void resetSettings();
        void begin();
        String getSensorInformationHtml();
};