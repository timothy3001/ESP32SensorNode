#include <WebServerSensor.h>

WebServerSensor::WebServerSensor(SensorBase *sensor)
{
    this->sensor = sensor;

    webServer = new AsyncWebServer(80);
    webServer->onNotFound([](AsyncWebServerRequest *request) { request->send(404, "text/plain", "Not found!"); });
}

void WebServerSensor::startWebServer()
{    
    webServer->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleRootPage(request); });
    webServer->on("/settings", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleSettingsPage(request); });
    webServer->on("/api/settings", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleGetSettings(request); });
    webServer->on("/api/settings", HTTP_POST, 
        [](AsyncWebServerRequest *request) -> void { request->send(400, "text/plain", "No data!"); },
        [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) -> void { request->send(400, "text/plain", "Wrong data!"); },
        [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) 
        { 
            this->handleUpdateSettings(request, data, len, index, total); 
        });
    webServer->on("/api/values", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleGetValues(request); });
    webServer->begin();    
}


void WebServerSensor::handleRootPage(AsyncWebServerRequest *request)
{
    
}

void WebServerSensor::handleSettingsPage(AsyncWebServerRequest *request)
{

}

void WebServerSensor::handleGetSettings(AsyncWebServerRequest *request)
{
    String settingsSensorString = sensor->getSettings();
    DynamicJsonDocument settingsSensorJson(1024);



}

void WebServerSensor::handleUpdateSettings(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (len != total)
    {
        request->send(400, "text/plain", "No settings found!");
    }
    else
    {
        DynamicJsonDocument doc(len);
        if (deserializeJson(doc, data) != DeserializationError::Ok)
        {
            request->send(400, "text/plain", "Could not parse JSON!");
        }
        else
        {
            if (doc.containsKey(GENERAL_PREFS_NAME))
            {
                this->updateGeneralSettings(doc[GENERAL_PREFS_NAME]);
            }

            String serialized;
            serializeJson(doc, serialized);

            sensor->updateSettings(serialized);
        }        
    }
}

void WebServerSensor::updateGeneralSettings(JsonObject generalSettingsObj)
{
    Preferences prefs;
    prefs.begin(GENERAL_PREFS_NAME, false);
    
    if (generalSettingsObj.containsKey(GENERAL_PREF_SENSOR_NAME))
        prefs.putString(GENERAL_PREF_SENSOR_NAME, generalSettingsObj[GENERAL_PREF_SENSOR_NAME].as<String>());
    if (generalSettingsObj.containsKey(GENERAL_PREF_BASE_ADDRESS))
        prefs.putString(GENERAL_PREF_BASE_ADDRESS, generalSettingsObj[GENERAL_PREF_BASE_ADDRESS].as<String>());
    if (generalSettingsObj.containsKey(GENERAL_PREF_BAT_REPORTING_ADDRESS))
        prefs.putString(GENERAL_PREF_BAT_REPORTING_ADDRESS, generalSettingsObj[GENERAL_PREF_BAT_REPORTING_ADDRESS].as<String>());
    if (generalSettingsObj.containsKey(GENERAL_PREF_ACTIVATE_REPORTING))
        prefs.putBool(GENERAL_PREF_ACTIVATE_REPORTING, generalSettingsObj[GENERAL_PREF_ACTIVATE_REPORTING].as<bool>());
    if (generalSettingsObj.containsKey(GENERAL_PREF_PASSIVE))
        prefs.putBool(GENERAL_PREF_PASSIVE, generalSettingsObj[GENERAL_PREF_PASSIVE].as<bool>());
    if (generalSettingsObj.containsKey(GENERAL_PREF_ACTIVATE_REPORTING_BAT))
        prefs.putBool(GENERAL_PREF_ACTIVATE_REPORTING_BAT, generalSettingsObj[GENERAL_PREF_ACTIVATE_REPORTING_BAT].as<bool>());
    if (generalSettingsObj.containsKey(GENERAL_PREF_REPORTING_INTERVAL_SECS))
        prefs.putInt(GENERAL_PREF_REPORTING_INTERVAL_SECS, generalSettingsObj[GENERAL_PREF_REPORTING_INTERVAL_SECS].as<int>());
    if (generalSettingsObj.containsKey(GENERAL_PREF_SENSOR_TYPE))
        prefs.putInt(GENERAL_PREF_SENSOR_TYPE, generalSettingsObj[GENERAL_PREF_SENSOR_TYPE].as<int>());

    prefs.end();
}

void WebServerSensor::handleGetValues(AsyncWebServerRequest *request)
{
    String valuesJson = sensor->getValues();
    request->send(200, "application/json", valuesJson);
}