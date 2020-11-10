#include <Arduino.h>
#include <EspWifiSetup.h>
#include <SensorBase.h>

const int PIN_BATTERY_MONITORING = 35;
const unsigned int TIMES_HALL_READ = 10;
const unsigned int DELAY_MS_HALL_READ = 100;
const unsigned int THRESHOLD_HALL = 30;

SensorBase* sensorKind = NULL;
unsigned long millisStart;

void initiateDeepSleepForReporting();
bool checkHallForReset();
void initSensor();

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

    // Initialize sensor
    initSensor();

    // Check if settings should be reset
    if (checkHallForReset())
    {
        Serial.println("Hall sensor threshold exceeded! Resetting settings...");
        EspWifiSetup::resetSettings();
        resetSettings();
    }

    Serial.println("Reading settings...");
    readSettings();
    Serial.println("Settings read!");

    Serial.println("Setting up wifi...");
    if (!EspWifiSetup::setup(String("Thermometer-") + settingName, false, settingPassive) && settingPassive)
    {
        initiateDeepSleepForReporting();
    }
    Serial.println("WiFi successfully set up!");

    Serial.println("Setting up sensors");
    sensors.begin();
    Serial.println("Sensors set up!");

    if (!settingPassive)
    {
        Serial.println("Setting up web server...");
        setupWebServer();
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

void initSensor() {
    // Read config
    sensorKind

}