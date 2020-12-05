#ifndef Constants_h_
#define Constants_h_

enum SensorType { None = 0, Thermometer = 1, Moisture = 2 };

const char* GENERAL_PREFS_NAME = "general";
const char* GENERAL_PREF_SENSOR_NAME = "name";
const char* GENERAL_PREF_ACTIVATE_REPORTING = "activateRep";
const char* GENERAL_PREF_BASE_ADDRESS = "baseAdd";
const char* GENERAL_PREF_REPORTING_INTERVAL_SECS = "intervalSecs";
const char* GENERAL_PREF_PASSIVE = "passive";
const char* GENERAL_PREF_ACTIVATE_REPORTING_BAT = "activateRepBat";
const char* GENERAL_PREF_BAT_REPORTING_ADDRESS = "batAdd";
const char* GENERAL_PREF_SENSOR_TYPE = "sensorType";
const char* SENSOR_PREFS_NAME = "sensor";

#endif