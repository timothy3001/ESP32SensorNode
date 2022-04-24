#include "SensorMoisturePlant.h"

void SensorMoisturePlant::begin()
{
    this->validSettings = readSettings();

    if (this->validSettings)
    {
        this->updateSensorValue();
    }
    else
    {
        this->logMessage("Could not initialize sensor, invalid settings found!");
    }
}

void SensorMoisturePlant::loop()
{
    unsigned long now = millis();

    if (lastUpdate + 5000 < now || lastUpdate > now) 
    {
        if (this->validSettings)
            this->updateSensorValue();
    }
}

void SensorMoisturePlant::executeReporting(String baseAddress)
{
    if (!this->validSettings)
    {
        logMessage("Settings are not valid, cannot execute reporting!");
    }
    else if (!this->isSensorValueValid())
    {
        logMessage("Current sensor value is invalid");
    }
    else if (!this->settingAddressValue.length() == 0)
    {
        logMessage("Address to report plant moisture to is not configured!");
    }
    else
    {
        HelperFunctions::sendPutRequest(
            baseAddress + String("/") + this->settingAddressValue,
            String(currentMoisturePercentage)
        );
    }
}

String SensorMoisturePlant::getConfigurationPageHtml()
{
    return sensorPlantMoistureSettings;
}

String SensorMoisturePlant::getSensorInformationHtml()
{
    return sensorPlantMoistureRoot;
}

String SensorMoisturePlant::getNamePrefix()
{
    return String("MoisturePlant");
}

String SensorMoisturePlant::getSettings()
{
    DynamicJsonDocument doc(1024);

    doc[SENSOR_MOISTURE_PLANT_PREF_DATA_PIN] = settingDataPinSensor;
    doc[SENSOR_MOISTURE_PLANT_PREF_REPORTING_ADDRESS] = settingAddressValue;
    doc[SENSOR_MOISTURE_PLANT_PREF_MAX_VALUE] = settingMaxValue;
    doc[SENSOR_MOISTURE_PLANT_PREF_MIN_VALUE] = settingMinValue;

    String result;
    serializeJson(doc, result);
    
    return result;
}

String SensorMoisturePlant::getValues()
{

}

void SensorMoisturePlant::resetSettings()
{

}

void SensorMoisturePlant::updateSettings(String settings)
{

}

void SensorMoisturePlant::logMessage(String msg)
{
    Serial.println(String("SensorMoisturePlant: ") + msg);
}

bool SensorMoisturePlant::readSettings()
{
    Preferences prefs;
    prefs.begin(SENSOR_PREFS_NAME, true);

    this->settingDataPinSensor = prefs.getInt(SENSOR_MOISTURE_PLANT_PREF_DATA_PIN, -1);
    this->settingAddressValue = prefs.getString(SENSOR_MOISTURE_PLANT_PREF_REPORTING_ADDRESS, String(""));
    this->settingMaxValue = prefs.getInt(SENSOR_MOISTURE_PLANT_PREF_MAX_VALUE, -1);
    this->settingMinValue = prefs.getInt(SENSOR_MOISTURE_PLANT_PREF_MIN_VALUE, -1);

    prefs.end();

    if (this->settingDataPinSensor >= 0)
        return true;

    return false;
}

bool SensorMoisturePlant::isSensorValueValid()
{
    if (currentMoistureAbsolute >= 0)
        return true;
}
