#include <SensorBase.h>

class SensorThermometer : public SensorBase
{
    public:
        void resetSettings();
        void begin();
        String getSensorInformationHtml();
};