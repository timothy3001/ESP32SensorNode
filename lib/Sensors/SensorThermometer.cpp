#include "SensorThermometer.h"

void SensorThermometer::begin()
{
    this->validSettings = readSettings();

    if (this->validSettings)
    {
        oneWire = new OneWire(settingDataPinSensor);
        tempSensor = new DallasTemperature(oneWire);

        this->updateTemperature();
    }
    else
    {
        this->logMessage("Could not initialize sensor, invalid settings found!");
    }
    
}

void SensorThermometer::resetSettings()
{
    Preferences prefs;
    prefs.begin(SENSOR_PREFS_NAME, false);
    prefs.clear();
    prefs.end();
}

String SensorThermometer::getSensorInformationHtml()
{
    return sensorThermometerRoot;
}

String SensorThermometer::getConfigurationPageHtml()
{
    return sensorThermometerSettings;
}

void SensorThermometer::updateSettings(String settings)
{
    DynamicJsonDocument doc(1024);
    DeserializationError deserializeResult = deserializeJson(doc, settings);

    if(deserializeResult != DeserializationError::Ok)
    {
        logMessage("Could not deserialize settings json! Deserialization code: " + String((int)deserializeResult.code()));
    }
    else
    {
        if (doc.containsKey(SENSOR_THERMOMETER_PREF_DATA_PIN))
            this->settingDataPinSensor = doc[SENSOR_THERMOMETER_PREF_DATA_PIN].as<int>();
        if (doc.containsKey(SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS))
        {
            String value = doc[SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS].as<String>();
            HelperFunctions::stripFirstSlash(value);
            this->settingAddressValue = value;
        }        
        
        Preferences prefs;
        prefs.begin(SENSOR_PREFS_NAME, false);

        prefs.putInt(SENSOR_THERMOMETER_PREF_DATA_PIN, this->settingDataPinSensor);
        prefs.putString(SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS, this->settingAddressValue);

        prefs.end();
    }    
}

String SensorThermometer::getValues()
{
    DynamicJsonDocument doc(256);

    if (!this->validSettings)
        doc["error"] = String("Could not get current temperature. Settings are invalid!");    
    else if (!this->isTempValid(this->currentTemperature))
        doc["error"] = String("Current temperature is invalid!");
    else
        doc["temperature"] = this->currentTemperature;
    
    String result;
    serializeJson(doc, result);

    return result;
}

String SensorThermometer::getSettings()
{
    DynamicJsonDocument doc(1024);

    doc[SENSOR_THERMOMETER_PREF_DATA_PIN] = settingDataPinSensor;
    doc[SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS] = settingAddressValue;

    String result;
    serializeJson(doc, result);
    
    return result;
}

void SensorThermometer::executeReporting(String baseAddress)
{
    if (!this->validSettings)
    {
        logMessage("Settings are not valid, cannot execute reporting!");
    }
    else if (!isTempValid(this->currentTemperature))
    {
        logMessage("Current temperature is invalid and thus will not be reported!");
    }
    else if (this->settingAddressValue.length() == 0)
    {
        logMessage("Address to report temperature to is not configured!");
    }
    else
    {
        HelperFunctions::sendPutRequest(
            baseAddress + String("/") + this->settingAddressValue,
            String(currentTemperature)
        );
    }   
}

void SensorThermometer::updateTemperature()
{
    int tries = 0;
    bool successful = false;
    while (tries < 5 && !successful)
    {
        tempSensor->requestTemperatures();
        float temp = tempSensor->getTempCByIndex(0);

        if (!isTempValid(temp))
        {
            this->logMessage(
                String("Could not read proper temperature on pin ") + 
                String(this->settingDataPinSensor) + 
                String("! Please check the wiring.")
            );
            delay(100);
        }
        else
        {
            this->currentTemperature = temp;
            logMessage(String("Current temperature: ") + String(this->currentTemperature) + String("°C"));
            successful = true;
        }
        tries++;
    }
}

bool SensorThermometer::isTempValid(float temperature)
{
    return (temperature > -30.0F && temperature < 57.0F);
}

bool SensorThermometer::readSettings()
{
    Preferences prefs;
    prefs.begin(SENSOR_PREFS_NAME, true);

    this->settingDataPinSensor = prefs.getInt(SENSOR_THERMOMETER_PREF_DATA_PIN, -1);
    this->settingAddressValue = prefs.getString(SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS, String(""));

    prefs.end();

    if (this->settingDataPinSensor >= 0)
        return true;
    
    return false;
}

void SensorThermometer::logMessage(String msg)
{
    Serial.println(String("SensorThermometer: ") + msg);
}

String SensorThermometer::getNamePrefix()
{
    return String("Thermometer");
}