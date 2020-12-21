#ifndef SensorCjmcu8128_h_
#define SensorCjmcu8128_h_

#include <Arduino.h>
#include <SensorBase.h>
#include <Preferences.h>
#include <Constants.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SensorCjmcu8128Root.h>
#include <SensorCjmcu8128Settings.h>
#include <SparkFunCCS811.h>
#include <SparkFunBME280.h>
#include <ClosedCube_HDC1080.h>
#include <HelperFunctions.h>
#include <math.h>

/**
 * 
 * The CJMCU8128 combines 3 sensor on one small PCB: CCS8111, BMP280 and HDC1080.
 * Some boards use another sensor instead of the HDC1080. This will not be taken into
 * account and you need to write another implementation.
 * 
 * CAUTION: For the I2C pins I use the suggested ones. SDA: GPIO21, SCL: GPIO22. I 
 * can highly recommend to do so because some libraries always call "Wire.begin()"
 * which uses the default pins. The behaviour of this is somewhat dangerous. But I
 * went for it so that I did not need to write a library for HDC1080. Just keep that 
 * in mind please.
 * 
 * Temperatures below -30°C or higher than 57°C are seen as invalid. Fahrenheit
 * is not supported by isTempValid. That function needs to be adjusted if using
 * Fahrenheit.
 */
class SensorCjmcu8128 : public SensorBase
{
    public:
        void begin();
        void executeReporting(String baseAddress);
        String getConfigurationPageHtml();
        String getSensorInformationHtml();
        String getNamePrefix();
        String getSettings();
        String getValues();
        void loop();
        void resetSettings();
        void updateSettings(String settings);
    
    private:
        const int CONTINOUS_UPDATE_INTERVAL = 10 * 1000; // 10 seconds
        const unsigned long CCS811_BURN_IN_TIME =  1000 * 60 * 20; // 20 minutes

        const char* SENSOR_CJMCU8128_PREF_SDA_PIN = "sdaPin";
        const char* SENSOR_CJMCU8128_PREF_SCL_PIN = "sclPin";
        const char* SENSOR_CJMCU8128_PREF_SCAN_DEVICES = "scanDevs";
        const char* SENSOR_CJMCU8128_PREF_CCS811_BASELINE = "ccsBase";
        const char* SENSOR_CJMCU8128_PREF_CCS811_CO2_SUBADDRESS = "ccsCo2";
        const char* SENSOR_CJMCU8128_PREF_CCS811_TVOC_SUBADDRESS = "ccsTvoc";
        const char* SENSOR_CJMCU8128_PREF_HDC1080_TEMP_SUBADDRESS = "hdcTemp";
        const char* SENSOR_CJMCU8128_PREF_HDC1080_HUMIDITY_SUBADDRESS = "hdcHum";
        const char* SENSOR_CJMCU8128_PREF_BMP280_TEMP_SUBADDRESS = "bmpTemp";
        const char* SENSOR_CJMCU8128_PREF_BMP280_PRESSURE_SUBADDRESS = "bmpPres";

        const int CCS811_ADDRESS = 0x5A;
        const int HDC1080_ADDRESS = 0x40;
        const int BMP280_ADDRESS = 0x76;

        void logMessage(String msg);
        bool readSettings();
        void saveSettings();
        void runDeviceScan();
        void updateReadings();
        void updateReadingCcs811();
        void updateReadingBmp280();
        void updateReadingHdc1080();
        bool isTempValid(float temp);

        int settingSdaPin;
        int settingSclPin;
        bool settingScanDevices;
        int settingCcs811Baseline;
        String settingCcs811Co2SubAddress;
        String settingCcs811TvocSubAddress;
        String settingHdc1080TempSubAddress;
        String settingHdc1080HumiditySubAddress;
        String settingBmp280TempSubAddress;
        String settingBmp280PressureSubAddress;

        bool validSettings = false;
        CCS811 *ccs811;
        bool ccs811Ready = false;
        int ccs811Co2 = -1;
        int ccs811Tvoc = -1;
        unsigned long lastUpdate = 0;
        ClosedCube_HDC1080 *hdc1080;
        bool hdc1080Ready = false;
        float hdc1080Temperature = -127.0f;
        float hdc1080Humidity = -1.0f;
        BME280 *bmp280;
        bool bmp280Ready = false;
        float bmp280Temperature = -127.0f;
        float bmp280Pressure = -1.0f;

};

#endif