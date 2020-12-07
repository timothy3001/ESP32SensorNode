#include "GeneralSettings.h"

void GeneralSettings::initialize()
{
    Preferences prefs;
    prefs.begin(GENERAL_PREFS_NAME, true);

    sensorName = prefs.getString(GENERAL_PREF_SENSOR_NAME, getShortMac());
    reportingActive = prefs.getBool(GENERAL_PREF_ACTIVATE_REPORTING, false);
    baseAddress = prefs.getString(GENERAL_PREF_BASE_ADDRESS, "");
    intervalSeconds = prefs.getUInt(GENERAL_PREF_REPORTING_INTERVAL_SECS, 1800);
    passiveOperation = prefs.getBool(GENERAL_PREF_PASSIVE, false);
    reportingBatteryActive = prefs.getBool(GENERAL_PREF_ACTIVATE_REPORTING_BAT, false);
    reportingBatteryAddress = prefs.getString(GENERAL_PREF_ACTIVATE_REPORTING_BAT, "");
    sensorType = (SensorType) prefs.getInt(GENERAL_PREF_SENSOR_TYPE, 0);

    prefs.end();
}

void GeneralSettings::save()
{
    Preferences prefs;
    prefs.begin(GENERAL_PREFS_NAME, false);

    prefs.putString(GENERAL_PREF_SENSOR_NAME, sensorName);
    prefs.putBool(GENERAL_PREF_ACTIVATE_REPORTING, reportingActive);
    prefs.putString(GENERAL_PREF_BASE_ADDRESS, baseAddress);
    prefs.putInt(GENERAL_PREF_REPORTING_INTERVAL_SECS, intervalSeconds);
    prefs.putBool(GENERAL_PREF_PASSIVE, passiveOperation);
    prefs.putBool(GENERAL_PREF_ACTIVATE_REPORTING_BAT, reportingBatteryActive);
    prefs.putString(GENERAL_PREF_BAT_REPORTING_ADDRESS, reportingBatteryAddress);
    prefs.putInt(GENERAL_PREF_SENSOR_TYPE, (int)sensorType);

    prefs.end();
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