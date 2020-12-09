#include "GeneralSettings.h"

void GeneralSettings::initialize()
{
    Preferences prefs;
    prefs.begin(GENERAL_PREFS_NAME, true);

    this->sensorName = prefs.getString(GENERAL_PREF_SENSOR_NAME, getShortMac());
    this->reportingActive = prefs.getBool(GENERAL_PREF_ACTIVATE_REPORTING, false);
    this->baseAddress = prefs.getString(GENERAL_PREF_BASE_ADDRESS, String(""));
    this->intervalSeconds = prefs.getUInt(GENERAL_PREF_REPORTING_INTERVAL_SECS, 1800);
    this->passiveOperation = prefs.getBool(GENERAL_PREF_PASSIVE, false);
    this->reportingBatteryActive = prefs.getBool(GENERAL_PREF_ACTIVATE_REPORTING_BAT, false);
    this->reportingBatteryAddress = prefs.getString(GENERAL_PREF_ACTIVATE_REPORTING_BAT, String(""));
    this->sensorType = (SensorType) prefs.getInt(GENERAL_PREF_SENSOR_TYPE, 0);

    prefs.end();
}

void GeneralSettings::save()
{
    Preferences prefs;
    prefs.begin(GENERAL_PREFS_NAME, false);

    HelperFunctions::stripLastSlash(this->baseAddress);
    HelperFunctions::stripFirstSlash(this->reportingBatteryAddress);

    prefs.putString(GENERAL_PREF_SENSOR_NAME, this->sensorName);
    prefs.putBool(GENERAL_PREF_ACTIVATE_REPORTING, this->reportingActive);
    prefs.putString(GENERAL_PREF_BASE_ADDRESS, this->baseAddress);
    prefs.putInt(GENERAL_PREF_REPORTING_INTERVAL_SECS, this->intervalSeconds);
    prefs.putBool(GENERAL_PREF_PASSIVE, this->passiveOperation);
    prefs.putBool(GENERAL_PREF_ACTIVATE_REPORTING_BAT, this->reportingBatteryActive);
    prefs.putString(GENERAL_PREF_BAT_REPORTING_ADDRESS, this->reportingBatteryAddress);
    prefs.putInt(GENERAL_PREF_SENSOR_TYPE, (int)this->sensorType);

    prefs.end();

    // Initializing again for a clean state
    initialize();
}

String GeneralSettings::getShortMac()
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    String macStringLastPart = "";
    for (int i = 4; i < 6; i++)
    {
        macStringLastPart += String(mac[i], HEX);
    }

    return macStringLastPart;
}

void GeneralSettings::resetSettings()
{
    Preferences prefs;
    prefs.begin(GENERAL_PREFS_NAME, false);
    prefs.clear();
    prefs.end();
}