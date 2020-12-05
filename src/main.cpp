#include <Arduino.h>
#include <EspWifiSetup.h>
#include <SensorBase.h>
#include <SensorMoisture.h>
#include <SensorThermometer.h>
#include <WebServerSensor.h>
#include <Constants.h>

const int PIN_BATTERY_MONITORING = 35;
const unsigned int TIMES_HALL_READ = 10;
const unsigned int DELAY_MS_HALL_READ = 100;
const unsigned int THRESHOLD_HALL = 30;

String settingSensorName;
bool settingActivateReporting;
String settingBaseAddress;
unsigned int settingIntervalSecs;
bool settingPassive;
bool settingReportBattery;
String settingReportBatteryAddress;
SensorType settingSensorType;

SensorBase* sensor = NULL;
WebServerSensor* webServer = NULL;
unsigned long millisStart;


void initiateDeepSleepForReporting();
bool checkHallForReset();
void initSensor();
void checkForReset();
void readSettings();
String getShortMac();
void resetSettings();

enum SensorType { Thermometer = 0, Moisture = 1 };

void setup()
{
    // Initialize and configuing analog readings
    Serial.begin(9600);
    analogSetAttenuation(ADC_11db);
    analogReadResolution(11);

    // Test reading battery
    delay(10);
    Serial.println("RAW:");
    Serial.println(String(analogRead(PIN_BATTERY_MONITORING)));

    millisStart = millis();

    // Read general prefs
    readGeneralPrefs();

    // Initialize sensor
    initSensor();

    // Check if settings should be reset
    checkForReset();

    Serial.println("Reading settings...");
    readSettings();
    Serial.println("Settings read!");

    // Connecting to WiFi. If WiFi is not set up at all the WiFi setup is started
    Serial.println("Setting up wifi...");
    if (!EspWifiSetup::setup(String("Sensor-") + settingSensorName, false, settingPassive) && settingPassive)
    {
        initiateDeepSleepForReporting();
    }
    Serial.println("WiFi successfully set up!");

    sensor->begin();

    if (!settingPassive)
    {
        Serial.println("Setting up web server...");
        webServer = new WebServerSensor(sensor);
        webServer->startWebServer();
        Serial.println("Webserver set up!");
    }
    else
    {
        Serial.println("Passive mode active");
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
}

void initiateDeepSleepForReporting()
{
    esp_sleep_enable_timer_wakeup(settingIntervalSecs * 1000 * 1000);
    esp_deep_sleep_start();
}

bool checkHallForReset()
{
    int sum = 0;
    for (int i = 0; i < TIMES_HALL_READ; i++)
    {
        sum += hallRead();
        delay(DELAY_MS_HALL_READ);
    }

    int hallValueMean = abs(sum / TIMES_HALL_READ);
    return (hallValueMean >= THRESHOLD_HALL);
}

void checkForReset() 
{
    if (checkHallForReset())
    {
        Serial.println("Hall sensor threshold exceeded! Resetting settings...");
        EspWifiSetup::resetSettings();
        resetSettings();

        if (sensor) {
            sensor->resetSettings();
        }
    }
}

void initSensor() 
{
    switch(settingSensorType)
    {
        case Thermometer:
            sensor = new SensorThermometer();
            break;
        case Moisture:
            sensor = new SensorMoisture();
            break;
    }
}

void readGeneralPrefs()
{

    Preferences prefs;
    prefs.begin(GENERAL_PREFS_NAME, true);

    settingSensorName = prefs.getString(GENERAL_PREF_SENSOR_NAME, getShortMac());
    settingActivateReporting = prefs.getBool(GENERAL_PREF_ACTIVATE_REPORTING, false);
    settingBaseAddress = prefs.getString(GENERAL_PREF_BASE_ADDRESS, "");
    settingIntervalSecs = prefs.getUInt(GENERAL_PREF_REPORTING_INTERVAL_SECS, 1800);
    settingPassive = prefs.getBool(GENERAL_PREF_PASSIVE, false);
    settingReportBattery = prefs.getBool(GENERAL_PREF_ACTIVATE_REPORTING_BAT, false);
    settingReportBatteryAddress = prefs.getString(GENERAL_PREF_ACTIVATE_REPORTING_BAT, "");
    settingSensorType = (SensorType) prefs.getInt(GENERAL_PREF_SENSOR_TYPE, 0);

    prefs.end();
}

String getShortMac()
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


void resetSettings()
{
    Preferences prefs;
    prefs.begin(GENERAL_PREFS_NAME, false);
    prefs.clear();
    prefs.end();
}