#include <WebServerSensor.h>

WebServerSensor::WebServerSensor(SensorBase *sensor, GeneralSettings *generalSettings)
{
    this->sensor = sensor;
    this->generalSettings = generalSettings;

    webServer = new AsyncWebServer(80);
    webServer->onNotFound([](AsyncWebServerRequest *request) { request->send(404, "text/plain", "Not found!"); });
}

void WebServerSensor::startWebServer(bool onlySettings)
{    
    if (onlySettings)
    {
        webServer->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleSettingsPage(request); });
    }
    else
    {
        webServer->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleRootPage(request); });
        webServer->on("/api/values", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleGetValues(request); });
    }
    
    webServer->on("/settings", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleSettingsPage(request); });
    webServer->on("/api/settings", HTTP_GET, [&](AsyncWebServerRequest *request) { this->handleGetSettings(request); });
    webServer->on("/api/settings", HTTP_POST, 
        [](AsyncWebServerRequest *request) -> void { request->send(400, "text/plain", "No data!"); },
        [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) -> void { request->send(400, "text/plain", "Wrong data!"); },
        [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) 
        { 
            this->handleUpdateSettings(request, data, len, index, total); 
        });
    webServer->on("/api/resetSettings", HTTP_POST, [&](AsyncWebServerRequest *request) { this->handleResetSettings(request); });
    webServer->begin();    
}

void WebServerSensor::handleRootPage(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->setCode(200);
    response->printf(rootPage, generalSettings->sensorName, sensor->getSensorInformationHtml());
    request->send(response);
}

void WebServerSensor::handleSettingsPage(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->setCode(200);
    response->printf(settingsPage, generalSettings->sensorName, sensor->getConfigurationPageHtml());
    request->send(response);
}

void WebServerSensor::handleGetSettings(AsyncWebServerRequest *request)
{    
    DynamicJsonDocument settingsDoc(1024);

    settingsDoc[GENERAL_PREFS_NAME][GENERAL_PREF_BASE_ADDRESS] = generalSettings->baseAddress;
    settingsDoc[GENERAL_PREFS_NAME][GENERAL_PREF_SENSOR_NAME] = generalSettings->sensorName;
    settingsDoc[GENERAL_PREFS_NAME][GENERAL_PREF_ACTIVATE_REPORTING] = generalSettings->reportingActive;
    settingsDoc[GENERAL_PREFS_NAME][GENERAL_PREF_ACTIVATE_REPORTING_BAT] = generalSettings->reportingBatteryActive;
    settingsDoc[GENERAL_PREFS_NAME][GENERAL_PREF_BAT_REPORTING_ADDRESS] = generalSettings->reportingBatteryAddress;
    settingsDoc[GENERAL_PREFS_NAME][GENERAL_PREF_REPORTING_INTERVAL_SECS] = generalSettings->intervalSeconds;
    settingsDoc[GENERAL_PREFS_NAME][GENERAL_PREF_PASSIVE] = generalSettings->passiveOperation;

    DynamicJsonDocument sensorSettingsDoc(1024);
    String sensorSettingsJson = sensor->getSettings();
    deserializeJson(sensorSettingsDoc, sensorSettingsJson);

    settingsDoc[SENSOR_PREFS_NAME] = sensorSettingsDoc.to<JsonObject>();

    String json;
    serializeJson(settingsDoc, json);
    
    request->send(200, "application/json", json);
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

            if (doc.containsKey(SENSOR_PREFS_NAME))
            {
                String serialized;
                serializeJson(doc[SENSOR_PREFS_NAME], serialized);

                sensor->updateSettings(serialized);
            }
        }        

        request->send(200, "text/plain", "");
        delay(200);

        // Restarting ESP after saving configuration
        ESP.restart();
        delay(500);
    }
}

void WebServerSensor::updateGeneralSettings(JsonObject generalSettingsObj)
{    
    if (generalSettingsObj.containsKey(GENERAL_PREF_SENSOR_NAME))
        generalSettings->sensorName = generalSettingsObj[GENERAL_PREF_SENSOR_NAME].as<String>();
    if (generalSettingsObj.containsKey(GENERAL_PREF_BASE_ADDRESS))
        generalSettings->baseAddress = generalSettingsObj[GENERAL_PREF_BASE_ADDRESS].as<String>();
    if (generalSettingsObj.containsKey(GENERAL_PREF_BAT_REPORTING_ADDRESS))
        generalSettings->reportingBatteryAddress = generalSettingsObj[GENERAL_PREF_BAT_REPORTING_ADDRESS].as<String>();
    if (generalSettingsObj.containsKey(GENERAL_PREF_ACTIVATE_REPORTING))
        generalSettings->reportingActive = generalSettingsObj[GENERAL_PREF_ACTIVATE_REPORTING].as<bool>();
    if (generalSettingsObj.containsKey(GENERAL_PREF_PASSIVE))
        generalSettings->passiveOperation = generalSettingsObj[GENERAL_PREF_PASSIVE].as<bool>();
    if (generalSettingsObj.containsKey(GENERAL_PREF_ACTIVATE_REPORTING_BAT))
        generalSettings->reportingBatteryActive = generalSettingsObj[GENERAL_PREF_ACTIVATE_REPORTING_BAT].as<bool>();
    if (generalSettingsObj.containsKey(GENERAL_PREF_REPORTING_INTERVAL_SECS))
        generalSettings->intervalSeconds = generalSettingsObj[GENERAL_PREF_REPORTING_INTERVAL_SECS].as<int>();

    generalSettings->save();
}

void WebServerSensor::handleGetValues(AsyncWebServerRequest *request)
{
    String valuesJson = sensor->getValues();
    request->send(200, "application/json", valuesJson);
}

void WebServerSensor::handleResetSettings(AsyncWebServerRequest *request)
{
    generalSettings->resetSettings();
    sensor->resetSettings();
    EspWifiSetup::resetSettings();
    
    request->send(200, "text/plain", "");
}