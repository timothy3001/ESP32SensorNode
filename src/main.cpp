#include <Arduino.h>
#include <EspWifiSetup.h>
#include <SensorBase.h>
#include <WebServerSensor.h>
#include <Constants.h>
#include <GeneralSettings.h>

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
SensorType settingSensorType = Thermometer;

SensorBase* sensor = NULL;
WebServerSensor* webServer = NULL;
GeneralSettings* generalSettings = NULL;
unsigned long millisStart;

void initiateDeepSleepForReporting();
bool checkHallForReset();
void initSensor();
void checkForReset();
String getShortMac();

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
    Serial.println("Reading general prefs...");
    generalSettings = new GeneralSettings();
    generalSettings->initialize();
    Serial.println("General prefs read!");

    // Initialize sensor
    Serial.println("Creating sensor type...");
    initSensor();
    Serial.println("Sensor type created!");

    // Check if settings should be reset
    Serial.println("Check if settings should be reset...");
    checkForReset();
    Serial.println("Check for settings reset done!");
        
    // Connecting to WiFi. If WiFi is not set up at all the WiFi setup is started
    Serial.println("Setting up wifi...");
    if (!EspWifiSetup::setup(String("Sensor-") + settingSensorName, false, settingPassive) && settingPassive)
    {
        initiateDeepSleepForReporting();
    }
    Serial.println("WiFi successfully set up!");

    if (settingSensorType == None)
    {
        Serial.println("Setting up web server for normal operation...");
        webServer = new WebServerSensor(sensor, generalSettings);
        webServer->startWebServer(true);
        Serial.println("Webserver set up for normal operation!");        
    }
    else if (settingPassive)
    {
        Serial.println("Passive mode active");

        // Do battery reporting

        // Call sensor reporting

        // Go to sleep again
    }
    else
    {
        Serial.println("Starting sensor...");
        sensor->begin();
        Serial.println("Sensor started!");

        Serial.println("Setting up web server for normal operation...");
        webServer = new WebServerSensor(sensor, generalSettings);
        webServer->startWebServer();
        Serial.println("Webserver set up for normal operation!");        
    }
}

void loop()
{
    sensor->loop();
    delay(1);
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
        generalSettings->resetSettings();

        if (sensor)        
            sensor->resetSettings();        
    }
}

void initSensor() 
{
    // switch(settingSensorType)
    // {
    //     case Thermometer:
    //         sensor = new SensorThermometer();
    //         break;
    //     case Moisture:
    //         sensor = new SensorMoisture();
    //         break;
    // }
}
