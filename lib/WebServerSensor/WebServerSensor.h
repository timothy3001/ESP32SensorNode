#ifndef WebServerSensor_h_
#define WebServerSensor_h

#include <Arduino.h>
#include <SensorBase.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Constants.h>
#include <ArduinoJson.h>
#include <Preferences.h>

class WebServerSensor
{
    public:
        WebServerSensor(SensorBase *sensor);
        ~WebServerSensor();
        void startWebServer();

    private:
        SensorBase *sensor = NULL;
        AsyncWebServer *webServer = NULL;

        void handleRootPage(AsyncWebServerRequest *request);
        void handleSettingsPage(AsyncWebServerRequest *request);
        void handleGetSettings(AsyncWebServerRequest *request);
        void handleUpdateSettings(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
        void handleGetValues(AsyncWebServerRequest *request);
        void updateGeneralSettings(JsonObject generalSettingsObj);
};

#endif
