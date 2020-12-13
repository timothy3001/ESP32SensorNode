#include <Arduino.h>
#include <EspWifiSetup.h>
#include <SensorBase.h>
#include <WebServerSensor.h>
#include <Constants.h>
#include <GeneralSettings.h>
#include <HelperFunctions.h>

// Define what kind of sensor you want to use. 
// Available options: Thermometer, MoisturePlants
#define SENSOR_KIND Thermometer

const int PIN_BATTERY_MONITORING = 35;
const unsigned int TIMES_HALL_READ = 10;
const unsigned int DELAY_MS_HALL_READ = 100;
const unsigned int THRESHOLD_HALL = 30;
const int NUMBER_SAMPLES_BATTERY = 10;

SensorBase* sensor = NULL;
WebServerSensor* webServer = NULL;
GeneralSettings* generalSettings = NULL;
unsigned long millisStart;

// Functions used
void initiateDeepSleepForReporting();
bool checkHallForReset();
void initSensor();
void checkForReset();
String getShortMac();
void handleReporting();
float getBatteryStatus();
void executeReporting();

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
    if (!EspWifiSetup::setup(String("Sensor-") + generalSettings->sensorName, false, generalSettings->passiveOperation) && 
        generalSettings->passiveOperation)
    {
        initiateDeepSleepForReporting();
    }
    Serial.println("WiFi successfully set up!");

    if (generalSettings->sensorType == None)
    {
        Serial.println("Setting up web server for normal operation...");
        webServer = new WebServerSensor(sensor, generalSettings);
        webServer->startWebServer(true);
        Serial.println("Webserver set up for normal operation!");        
    }
    else if (generalSettings->passiveOperation)
    {
        Serial.println("Passive mode active");
        executeReporting();

        Serial.println("Initiating deep sleep...");
        initiateDeepSleepForReporting();

        delay(500);
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
    handleReporting();
    delay(1);
}

void initiateDeepSleepForReporting()
{
    esp_sleep_enable_timer_wakeup(generalSettings->intervalSeconds * 1000 * 1000);
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

unsigned long lastReportingChecked = 0;
void handleReporting()
{
    if (generalSettings->reportingActive)
    {
        unsigned long now = millis();
        if (lastReportingChecked == 0 || lastReportingChecked + generalSettings->intervalSeconds * 1000 < now || lastReportingChecked > now)
        {
            lastReportingChecked = millis();
            executeReporting();
        }
    }
}

void executeReporting()
{    
    Serial.println("Reporting executing...");

    if (generalSettings->reportingBatteryActive)
    {
        float currentBatteryStatus = getBatteryStatus();

        if (generalSettings->reportingBatteryAddress.length() == 0)
        {
            Serial.println("Invalid setting for address battery found!");
        }
        else if (currentBatteryStatus < 0.0F)
        {
            Serial.println(String("Invalid battery status found! Cannot finish reporting successfully!"));
        }
        else
        {
            HelperFunctions::sendPutRequest(
                generalSettings->baseAddress + 
                String("/") + 
                generalSettings->reportingBatteryAddress, 
                String(currentBatteryStatus)
            );
            Serial.println(String("Reporting battery finished!"));
        }
    }

    sensor->executeReporting(generalSettings->baseAddress);

    Serial.println("Reporting done!");
}

float getBatteryStatus()
{    
    int sumValues = 0;
    for (int i = 0; i < NUMBER_SAMPLES_BATTERY; i++)
    {
        sumValues += analogRead(PIN_BATTERY_MONITORING);
        delay(10);
    }

    float value = (float)sumValues / (float)NUMBER_SAMPLES_BATTERY;
    float batteryVoltage = ((float)value / 2047.0F) * 4.2F;

    float currentBatteryStatus = (batteryVoltage - (2.8F)) / (4.2F - 2.8F);

    Serial.println(String("Current battery level: ") + String(currentBatteryStatus * 100) + String("% (") + String(batteryVoltage) + String(" V) Raw: ") + String(value));
    Serial.println(value);

    return currentBatteryStatus;
}