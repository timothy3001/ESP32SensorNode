#include <Arduino.h>
#include <SensorBase.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WebServerSensor
{
    public:
        WebServerSensor(SensorBase *sensor);
        ~WebServerSensor();
        void startConfigurationServer();
        void startNormalOperationServer();

    private:
        SensorBase *sensor = NULL;
        AsyncWebServer *webServer = NULL;

        void handleRootPageSetting(AsyncWebServerRequest *request);
        void handleRootPageNormalOperation(AsyncWebServerRequest *request);
};