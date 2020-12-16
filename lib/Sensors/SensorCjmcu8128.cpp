#include "SensorCjmcu8128.h"

void SensorCjmcu8128::begin()
{
    this->validSettings = readSettings();

    if (this->validSettings)
    {
        Wire.begin(settingSdaPin, settingSclPin);

        if (this->settingScanDevices)
        {
            this->runDeviceScan();
        }

        // Setting up CCS811 sensor
        this->ccs811 = new CCS811(CCS811_ADDRESS);
        this->ccs811Ready = ccs811->begin(Wire);
        if (!ccs811Ready)
        {
            logMessage("ERROR - Could not setup CCS811, please check wiring and address!");
        }      

        // Setting up BMP280 sensor
        this->bmp280 = new BME280();
        this->bmp280->setI2CAddress(BMP280_ADDRESS);
        this->bmp280Ready = this->bmp280->beginI2C(Wire);
        if(!bmp280Ready)
        {
            logMessage("ERROR - could setup BMP280, please check wiring and address!");
        }

        // Setting up HDC1080
        this->hdc1080 = new ClosedCube_HDC1080();
        this->hdc1080->begin(HDC1080_ADDRESS);

        this->updateReadings();
    }
}

void SensorCjmcu8128::updateReadings()
{
    if(this->ccs811Ready)
        this->updateReadingCcs811();
    if(this->hdc1080Ready)
        this->updateReadingHdc1080();
    if (this->bmp280Ready)
        this->updateReadingBmp280();
}

void SensorCjmcu8128::updateReadingCcs811() 
{
    unsigned long firstTryReading = millis();

    while(firstTryReading + 10000 > millis())
    {
        delay(20);

        if (this->ccs811->dataAvailable())
        {
            this->ccs811->readAlgorithmResults();

            delay(20);

            this->ccs811Tvoc = this->ccs811->getTVOC();
            this->ccs811Co2 = this->ccs811->getCO2();

            // Sensor delivers useful readings after 3 readings. So let's wait
            // for the useful readings
            if (this->ccs811Co2 != 0)
            {
                logMessage(
                    String("Reading CCS811 -") +
                    String(" TVOC: ") + String(this->ccs811Tvoc) + 
                    String(" CO2: " + String(this->ccs811Co2))
                );
            }
        }
    }    

    if (firstTryReading + 10000 < millis())
    {
        logMessage("ERROR - Could not read data from CCS811 sensor!");
    }
}

void SensorCjmcu8128::updateReadingBmp280()
{
    this->bmp280Temperature = this->bmp280->readTempC();
    this->bmp280Pressure = this->bmp280->readFloatPressure();

    logMessage(
        String("Reading BMP280 -") +
        String(" Temperature: ") + String(this->bmp280Temperature) +
        String(" Pressure: ") + String(this->bmp280Pressure)
    );
}

void SensorCjmcu8128::updateReadingHdc1080()
{
    this->hdc1080Temperature = (float)this->hdc1080->readTemperature();
    this->hdc1080Humidity = (float)this->hdc1080->readHumidity();

    logMessage(
        String("Reading HDC1080 -") +
        String(" Temperature: ") + String(this->hdc1080Temperature) +
        String(" Humidity: ") + String(this->hdc1080Humidity)
    );
}

void SensorCjmcu8128::executeReporting(String baseAddress)
{
    logMessage("executingReporting() not implemented yet!");
}

void SensorCjmcu8128::loop()
{
    unsigned long now = millis();

    if (lastUpdate + CONTINOUS_UPDATE_INTERVAL < now || lastUpdate > now) 
    {
        if (this->validSettings)
            this->updateReadings();
    }
}

String SensorCjmcu8128::getConfigurationPageHtml() 
{
    return sensorCjmcu8128Settings;
}

String SensorCjmcu8128::getSensorInformationHtml()
{
    return sensorCjmcu8128Root;
}

String SensorCjmcu8128::getNamePrefix()
{
    return String("CJMCU8128");
}

String SensorCjmcu8128::getValues()
{
    // ToDo: Implement
    return String("");
}

String SensorCjmcu8128::getSettings()
{
    DynamicJsonDocument doc(1024);

    doc[SENSOR_CJMCU8128_PREF_SDA_PIN] = settingSdaPin;
    doc[SENSOR_CJMCU8128_PREF_SCL_PIN] = settingSclPin;
    doc[SENSOR_CJMCU8128_PREF_SCAN_DEVICES] = settingScanDevices;

    String result;
    serializeJson(doc, result);
    
    return result;
}

void SensorCjmcu8128::resetSettings()
{
    Preferences prefs;
    prefs.begin(SENSOR_PREFS_NAME, false);
    prefs.clear();
    prefs.end();
}

void SensorCjmcu8128::updateSettings(String settings)
{
    DynamicJsonDocument doc(1024);
    DeserializationError deserializeResult = deserializeJson(doc, settings);

    if(deserializeResult != DeserializationError::Ok)
    {
        logMessage("Could not deserialize settings json! Deserialization code: " + String((int)deserializeResult.code()));
    }
    else
    {
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_SDA_PIN))
            this->settingSdaPin = doc[SENSOR_CJMCU8128_PREF_SDA_PIN].as<int>();
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_SCL_PIN))
            this->settingSclPin = doc[SENSOR_CJMCU8128_PREF_SCL_PIN].as<int>();
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_SCAN_DEVICES))
            this->settingScanDevices = doc[SENSOR_CJMCU8128_PREF_SCAN_DEVICES].as<bool>();

        // if (doc.containsKey(SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS))
        // {
        //     String value = doc[SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS].as<String>();
        //     HelperFunctions::stripFirstSlash(value);
        //     this->settingAddressValue = value;
        // }        
        
        Preferences prefs;
        prefs.begin(SENSOR_PREFS_NAME, false);

        prefs.putInt(SENSOR_CJMCU8128_PREF_SDA_PIN, this->settingSdaPin);
        prefs.putInt(SENSOR_CJMCU8128_PREF_SCL_PIN, this->settingSclPin);
        prefs.putBool(SENSOR_CJMCU8128_PREF_SCAN_DEVICES, this->settingScanDevices);

        // prefs.putString(SENSOR_THERMOMETER_PREF_REPORTING_ADDRESS, this->settingAddressValue);

        prefs.end();
    }
}

void SensorCjmcu8128::logMessage(String msg)
{
    Serial.println(String("SensorCJMCU8128: ") + msg);
}

bool SensorCjmcu8128::readSettings()
{
    Preferences prefs;
    prefs.begin(SENSOR_PREFS_NAME, true);

    this->settingSdaPin = prefs.getInt(SENSOR_CJMCU8128_PREF_SDA_PIN, -1);
    this->settingSclPin = prefs.getInt(SENSOR_CJMCU8128_PREF_SCL_PIN, -1);
    this->settingScanDevices = prefs.getBool(SENSOR_CJMCU8128_PREF_SCAN_DEVICES, true);

    prefs.end();

    if (this->settingSdaPin >= 0 && this->settingSclPin >= 0)
        return true;
    
    return false;
}

void SensorCjmcu8128::runDeviceScan()
{
    Wire.begin(settingSdaPin, settingSclPin);

    // See https://create.arduino.cc/projecthub/abdularbi17/how-to-scan-i2c-address-in-arduino-eaadda 
    byte error;
    byte address;
    int numberDevices = 0;

    logMessage("Starting scan for I2C devices...");

    for (address = 1; address < 127; address++)
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            String addressString = String("0x");
            if (address < 16)
                addressString += String("0");
            addressString += String(address, HEX);
            
            logMessage(String("I2C device found at address ") + addressString);
            numberDevices++;
        }
        else if (error == 4)
        {
            String addressString = String("0x");
            if (address < 16)
                addressString += String("0");
            addressString += String(address, HEX);

           logMessage(String("Unknown error at address ") + addressString);
        }
    }

    if (numberDevices == 0)
        Serial.println("Could not find any I2C devices, check the wiring!");

    logMessage("Finished scan for I2C devices");
}