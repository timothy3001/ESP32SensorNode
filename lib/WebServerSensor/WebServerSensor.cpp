#include <WebServerSensor.h>

WebServerSensor::WebServerSensor(SensorBase *sensor)
{
    this->sensor = sensor;

    webServer = new AsyncWebServer(80);
    webServer->onNotFound([](AsyncWebServerRequest *request) { request->send(404, "text/plain", "Not found!"); });
}

void WebServerSensor::startConfigurationServer()
{
    
    webServer->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) { handleRootPageSetting(request); });
    webServer->begin();    
}

void WebServerSensor::startNormalOperationServer()
{
    webServer->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) { handleRootPageNormalOperation(request); });
    webServer->begin();    
}

void WebServerSensor::handleRootPageSetting(AsyncWebServerRequest *request)
{

}

void WebServerSensor::handleRootPageNormalOperation(AsyncWebServerRequest *request)
{
    
}