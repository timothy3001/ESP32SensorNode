#ifndef Constants_h_
#define Constants_h_

enum SensorType { None = 0, Thermometer = 1, MoisturePlants = 2, Cjmcu8128 = 3 };

const char* const GENERAL_PREFS_NAME = "general";
const char* const GENERAL_PREF_SENSOR_NAME = "name";
const char* const GENERAL_PREF_ACTIVATE_REPORTING = "activateRep";
const char* const GENERAL_PREF_BASE_ADDRESS = "baseAdd";
const char* const GENERAL_PREF_REPORTING_INTERVAL_SECS = "intervalSecs";
const char* const GENERAL_PREF_PASSIVE = "passive";
const char* const GENERAL_PREF_ACTIVATE_REPORTING_BAT = "activateRepBat";
const char* const GENERAL_PREF_BAT_REPORTING_ADDRESS = "batAdd";
const char* const GENERAL_PREF_FIRST_RUN = "firstRun";
const char* const SENSOR_PREFS_NAME = "sensor";

#endif