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
        else if (this->settingCcs811Baseline >= 0)
        {
            this->ccs811->setBaseline(this->settingCcs811Baseline);
        }

        // Setting up BMP280 sensor
        this->bmp280 = new BME280();
        this->bmp280->setI2CAddress(BMP280_ADDRESS);
        this->bmp280Ready = this->bmp280->beginI2C(Wire);
        if(!bmp280Ready)
        {
            logMessage("ERROR - could not setup BMP280, please check wiring and address!");
        }

        // Setting up HDC1080
        this->hdc1080 = new ClosedCube_HDC1080();
        this->hdc1080->begin(HDC1080_ADDRESS);
        this->hdc1080Ready = true;

        this->updateReadings();
    }
}

void SensorCjmcu8128::updateReadings()
{
    if (this->bmp280Ready)
        this->updateReadingBmp280();
    if(this->hdc1080Ready)
        this->updateReadingHdc1080();
    // Make sure that HDC1080 reading was done before running reading of CCS811
    // so that the environmental data for measuring TVOC is set correctly.
    if(this->ccs811Ready)  
        this->updateReadingCcs811();    
    
}

void SensorCjmcu8128::updateReadingCcs811() 
{
    unsigned long firstTryReading = millis();

    if (this->hdc1080Ready && 
        this->isTempValid(this->hdc1080Temperature) &&
        this->hdc1080Humidity >= 0.0f)
    {
        this->ccs811->setEnvironmentalData(this->hdc1080Humidity, this->hdc1080Temperature);
    }

    while(firstTryReading + 11000 > millis())
    {
        delay(100);
        
        if (this->ccs811->dataAvailable())
        {
            this->ccs811->readAlgorithmResults();

            delay(20);

            this->ccs811Tvoc = this->ccs811->getTVOC();
            this->ccs811Co2 = this->ccs811->getCO2();

            // Sensor delivers useful readings after 3 readings. So let's wait
            // for the 4th, the useful reading
            if (this->ccs811Co2 != 0)
            {
                logMessage(
                    String("Reading CCS811 -") +
                    String(" TVOC: ") + String(this->ccs811Tvoc) + 
                    String(" CO2: " + String(this->ccs811Co2))
                );

                break;
            }
        }
    }    

    if (firstTryReading + 11000 < millis())
    {
        logMessage("ERROR - Could not read data from CCS811 sensor!");
    }
    else
    {
        // Set drive mode to 2. This is slowing down readings to every 10
        // seconds. This is so that the board does not heat up due to heating
        // element.
        this->ccs811->setDriveMode(2);
    }
    
}

void SensorCjmcu8128::updateReadingBmp280()
{
    this->bmp280Temperature = round((float)this->bmp280->readTempC() * 10) / 10;
    this->bmp280Pressure = round(((float)this->bmp280->readFloatPressure() / 100) * 10) / 10;

    logMessage(
        String("Reading BMP280 -") +
        String(" Temperature: ") + String(this->bmp280Temperature) +
        String(" Pressure: ") + String(this->bmp280Pressure)
    );
}

void SensorCjmcu8128::updateReadingHdc1080()
{
    this->hdc1080Temperature = round((float)this->hdc1080->readTemperature() * 10) / 10;
    this->hdc1080Humidity = round((float)this->hdc1080->readHumidity() * 10) / 10;

    logMessage(
        String("Reading HDC1080 -") +
        String(" Temperature: ") + String(this->hdc1080Temperature) +
        String(" Humidity: ") + String(this->hdc1080Humidity)
    );
}

void SensorCjmcu8128::executeReporting(String baseAddress)
{
    if (!this->validSettings)
    {
        logMessage("Settings are not valid, cannot execute reporting!");
        return;
    }

    if (this->settingCcs811Co2SubAddress.length() > 0 && this->ccs811Co2 >= 0)
        HelperFunctions::sendPutRequest(baseAddress + String("/") + this->settingCcs811Co2SubAddress,
        String(this->ccs811Co2));
    if (this->settingCcs811TvocSubAddress.length() > 0 && this->ccs811Tvoc >= 0)
        HelperFunctions::sendPutRequest(baseAddress + String("/") + this->settingCcs811TvocSubAddress,
        String(this->ccs811Tvoc));
    if (this->settingHdc1080TempSubAddress.length() > 0 && this->isTempValid(this->hdc1080Temperature))
        HelperFunctions::sendPutRequest(baseAddress + String("/") + this->settingHdc1080TempSubAddress,
        String(this->hdc1080Temperature));
    if (this->settingHdc1080HumiditySubAddress.length() > 0 && this->hdc1080Humidity >= 0.0f)
        HelperFunctions::sendPutRequest(baseAddress + String("/") + this->settingHdc1080HumiditySubAddress,
        String(this->hdc1080Humidity));
    if (this->settingBmp280TempSubAddress.length() > 0 && this->isTempValid(this->bmp280Temperature))
        HelperFunctions::sendPutRequest(baseAddress + String("/") + this->settingBmp280TempSubAddress,
        String(this->bmp280Temperature));
    if (this->settingBmp280PressureSubAddress.length() > 0 && this->bmp280Pressure >= 0.0f)
        HelperFunctions::sendPutRequest(baseAddress + String("/") + this->settingBmp280PressureSubAddress,
        String(this->bmp280Pressure));
}

void SensorCjmcu8128::loop()
{
    unsigned long now = millis();

    if ((lastUpdate + CONTINOUS_UPDATE_INTERVAL < now || lastUpdate > now) && this->validSettings)
    {
        this->updateReadings();
        
        if (this->settingCcs811Baseline == -1 && this->ccs811Ready && now > CCS811_BURN_IN_TIME)
        {
            this->settingCcs811Baseline = this->ccs811->getBaseline();
            logMessage(String("Set CCS811 baseline to: ") + String(this->settingCcs811Baseline));
            this->saveSettings();
        }

        this->lastUpdate = millis();
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
    DynamicJsonDocument doc(1024);

    if (!this->validSettings)
        doc["error"] = String("Could not get current values. Settings are invalid!");    
    else
    {
        if (this->ccs811Co2 >= 0)
            doc["co2"] = this->ccs811Co2;
        if (this->ccs811Tvoc >= 0)
            doc["tvoc"] = this->ccs811Tvoc;
        if (this->isTempValid(this->hdc1080Temperature))
            doc["tempHdc1080"] = this->hdc1080Temperature;
        if (this->hdc1080Humidity >= 0.0f)
            doc["humidity"] = this->hdc1080Humidity;
        if (this->isTempValid(this->bmp280Temperature))
            doc["tempBmp280"] = this->bmp280Temperature;
        if (this->bmp280Pressure >= 0.0f)
            doc["pressure"] = this->bmp280Pressure;
    }
    
    
    String result;
    serializeJson(doc, result);

    return result;
}

String SensorCjmcu8128::getSettings()
{
    DynamicJsonDocument doc(1024);

    doc[SENSOR_CJMCU8128_PREF_SDA_PIN] = settingSdaPin;
    doc[SENSOR_CJMCU8128_PREF_SCL_PIN] = settingSclPin;
    doc[SENSOR_CJMCU8128_PREF_SCAN_DEVICES] = settingScanDevices;
    doc[SENSOR_CJMCU8128_PREF_CCS811_BASELINE] = settingCcs811Baseline;
    doc[SENSOR_CJMCU8128_PREF_CCS811_CO2_SUBADDRESS] = settingCcs811Co2SubAddress;
    doc[SENSOR_CJMCU8128_PREF_CCS811_TVOC_SUBADDRESS] = settingCcs811TvocSubAddress;
    doc[SENSOR_CJMCU8128_PREF_HDC1080_HUMIDITY_SUBADDRESS] = settingHdc1080HumiditySubAddress;
    doc[SENSOR_CJMCU8128_PREF_HDC1080_TEMP_SUBADDRESS] = settingHdc1080TempSubAddress;
    doc[SENSOR_CJMCU8128_PREF_BMP280_TEMP_SUBADDRESS] = settingBmp280TempSubAddress;
    doc[SENSOR_CJMCU8128_PREF_BMP280_PRESSURE_SUBADDRESS] = settingBmp280PressureSubAddress;

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
    DynamicJsonDocument doc(2048);
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
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_CCS811_BASELINE))
            this->settingCcs811Baseline = doc[SENSOR_CJMCU8128_PREF_CCS811_BASELINE].as<int>();
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_CCS811_CO2_SUBADDRESS))
        {
            String value = doc[SENSOR_CJMCU8128_PREF_CCS811_CO2_SUBADDRESS].as<String>();
            value = HelperFunctions::stripFirstSlash(value);
            this->settingCcs811Co2SubAddress = value;
        }
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_CCS811_TVOC_SUBADDRESS))
        {
            String value = doc[SENSOR_CJMCU8128_PREF_CCS811_TVOC_SUBADDRESS].as<String>();
            value = HelperFunctions::stripFirstSlash(value);
            this->settingCcs811TvocSubAddress = value;
        }
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_HDC1080_TEMP_SUBADDRESS))
        {
            String value = doc[SENSOR_CJMCU8128_PREF_HDC1080_TEMP_SUBADDRESS].as<String>();
            value = HelperFunctions::stripFirstSlash(value);
            this->settingHdc1080TempSubAddress = value;
        }
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_HDC1080_HUMIDITY_SUBADDRESS))
        {
            String value = doc[SENSOR_CJMCU8128_PREF_HDC1080_HUMIDITY_SUBADDRESS].as<String>();
            value = HelperFunctions::stripFirstSlash(value);
            this->settingHdc1080HumiditySubAddress = value;
        }
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_BMP280_TEMP_SUBADDRESS))
        {
            String value = doc[SENSOR_CJMCU8128_PREF_BMP280_TEMP_SUBADDRESS].as<String>();
            value = HelperFunctions::stripFirstSlash(value);
            this->settingBmp280TempSubAddress = value;
        }
        if (doc.containsKey(SENSOR_CJMCU8128_PREF_BMP280_PRESSURE_SUBADDRESS))
        {
            String value = doc[SENSOR_CJMCU8128_PREF_BMP280_PRESSURE_SUBADDRESS].as<String>();
            value = HelperFunctions::stripFirstSlash(value);
            this->settingBmp280PressureSubAddress = value;
        }

        this->saveSettings();
    }
}

void SensorCjmcu8128::saveSettings()
{
    Preferences prefs;
    prefs.begin(SENSOR_PREFS_NAME, false);

    prefs.putInt(SENSOR_CJMCU8128_PREF_SDA_PIN, this->settingSdaPin);
    prefs.putInt(SENSOR_CJMCU8128_PREF_SCL_PIN, this->settingSclPin);
    prefs.putBool(SENSOR_CJMCU8128_PREF_SCAN_DEVICES, this->settingScanDevices);
    prefs.putInt(SENSOR_CJMCU8128_PREF_CCS811_BASELINE, this->settingCcs811Baseline);
    prefs.putString(SENSOR_CJMCU8128_PREF_CCS811_CO2_SUBADDRESS, this->settingCcs811Co2SubAddress);
    prefs.putString(SENSOR_CJMCU8128_PREF_CCS811_TVOC_SUBADDRESS, this->settingCcs811TvocSubAddress);
    prefs.putString(SENSOR_CJMCU8128_PREF_HDC1080_TEMP_SUBADDRESS, this->settingHdc1080TempSubAddress);
    prefs.putString(SENSOR_CJMCU8128_PREF_HDC1080_HUMIDITY_SUBADDRESS, this->settingHdc1080HumiditySubAddress);
    prefs.putString(SENSOR_CJMCU8128_PREF_BMP280_TEMP_SUBADDRESS, this->settingBmp280TempSubAddress);
    prefs.putString(SENSOR_CJMCU8128_PREF_BMP280_PRESSURE_SUBADDRESS, this->settingBmp280PressureSubAddress);

    prefs.end();
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
    this->settingCcs811Baseline = prefs.getInt(SENSOR_CJMCU8128_PREF_CCS811_BASELINE, -1);
    this->settingCcs811Co2SubAddress = prefs.getString(SENSOR_CJMCU8128_PREF_CCS811_CO2_SUBADDRESS, String(""));
    this->settingCcs811TvocSubAddress = prefs.getString(SENSOR_CJMCU8128_PREF_CCS811_TVOC_SUBADDRESS, String(""));
    this->settingHdc1080TempSubAddress = prefs.getString(SENSOR_CJMCU8128_PREF_HDC1080_TEMP_SUBADDRESS, String(""));
    this->settingHdc1080HumiditySubAddress = prefs.getString(SENSOR_CJMCU8128_PREF_HDC1080_HUMIDITY_SUBADDRESS, String(""));
    this->settingBmp280TempSubAddress = prefs.getString(SENSOR_CJMCU8128_PREF_BMP280_TEMP_SUBADDRESS, String(""));
    this->settingBmp280PressureSubAddress = prefs.getString(SENSOR_CJMCU8128_PREF_BMP280_PRESSURE_SUBADDRESS, String(""));

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

bool SensorCjmcu8128::isTempValid(float temperature)
{
    return (temperature > -30.0F && temperature < 57.0F);
}