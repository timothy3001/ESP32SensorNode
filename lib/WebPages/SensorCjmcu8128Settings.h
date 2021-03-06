#include <Arduino.h>

const char sensorCjmcu8128Settings[] PROGMEM = R"=====(
  <script>
    var inputSensorSdaPin;
    var inputSensorSclPin;
    var inputActivateScanDevices;
    var inputCcs811Baseline;
    var inputCcs811Co2Subaddress;
    var inputCcs811TvocSubaddress;
    var inputCcs811ConstBaselineUpdate;
    var inputHdc1080TempSubaddress;
    var inputHdc1080HumiditySubaddress;
    var inputHdcBmp280TempSubaddress;
    var inputHdcBmp280PressureSubaddress;
    var inputSensorDs18b20Pin;
    var inputDs18b20TempSubaddress;
  
    function sensorOnload() {
      inputSensorSdaPin = document.getElementById('inputSensorSdaPin');
      inputSensorSclPin = document.getElementById('inputSensorSclPin');
      inputActivateScanDevices = document.getElementById('inputActivateScanDevices');
      inputCcs811Baseline = document.getElementById('inputCcs811Baseline');
      inputCcs811Co2Subaddress = document.getElementById('inputCcs811Co2Subaddress');
      inputCcs811TvocSubaddress = document.getElementById('inputCcs811TvocSubaddress');
      inputCcs811ConstBaselineUpdate = document.getElementById('inputCcs811ConstBaselineUpdate');
      inputHdc1080TempSubaddress = document.getElementById('inputHdc1080TempSubaddress');
      inputHdc1080HumiditySubaddress = document.getElementById('inputHdc1080HumiditySubaddress');
      inputHdcBmp280TempSubaddress = document.getElementById('inputHdcBmp280TempSubaddress');
      inputHdcBmp280PressureSubaddress = document.getElementById('inputHdcBmp280PressureSubaddress');
      inputSensorDs18b20Pin = document.getElementById('inputSensorDs18b20Pin');
      inputDs18b20TempSubaddress = document.getElementById('inputDs18b20TempSubaddress');
    }
  
    function displayCurrentSensorSettings(settings) {
      inputSensorSdaPin.value = settings['sdaPin'];
      inputSensorSclPin.value = settings['sclPin'];
      inputActivateScanDevices.checked = settings['scanDevs'];
      inputCcs811Baseline.value = settings['ccsBase'];
      inputCcs811Co2Subaddress.value = settings['ccsCo2'];
      inputCcs811TvocSubaddress.value = settings['ccsTvoc'];
      inputCcs811ConstBaselineUpdate.checked = settings['ccsBaseUp']
      inputHdc1080TempSubaddress.value = settings['hdcTemp'];
      inputHdc1080HumiditySubaddress.value = settings['hdcHum'];
      inputHdcBmp280TempSubaddress.value = settings['bmpTemp'];
      inputHdcBmp280PressureSubaddress.value = settings['bmpPres'];
      inputSensorDs18b20Pin.value = settings['dsPin'];
      inputDs18b20TempSubaddress.value = settings['dsTemp'];
      
      inputSensorSdaPin.disabled = false;
      inputSensorSclPin.disabled = false;
      inputActivateScanDevices.disabled = false;
      inputCcs811Baseline.disabled = false;
      inputCcs811Co2Subaddress.disabled = false;
      inputCcs811TvocSubaddress.disabled = false;
      inputCcs811ConstBaselineUpdate.disabled = false;
      inputHdc1080TempSubaddress.disabled = false;
      inputHdc1080HumiditySubaddress.disabled = false;
      inputHdcBmp280TempSubaddress.disabled = false;
      inputHdcBmp280PressureSubaddress.disabled = false;
      inputSensorDs18b20Pin.disabled = false;
      inputDs18b20TempSubaddress.disabled = false;
    }
  
    function getSensorSettings() {
      if (inputSensorSdaPin.value < 0 || inputSensorSdaPin.value > 42)
      {
        window.alert('Please enter a valid SDA pin for connection to CJMCU8128!');
        return undefined;
      }
      if (inputSensorSclPin.value < 0 || inputSensorSclPin.value > 42)
      {
        window.alert('Please enter a valid SCL pin for connection to CJMCU8128!');
        return undefined;
      }
  
      var sensorSettingsObj = {
        'sdaPin': parseInt(inputSensorSdaPin.value),
        'sclPin': parseInt(inputSensorSclPin.value),
        'scanDevs': inputActivateScanDevices.checked,
        'ccsBase': inputCcs811Baseline.value,
        'ccsCo2': inputCcs811Co2Subaddress.value,
        'ccsTvoc': inputCcs811TvocSubaddress.value,
        'ccsBaseUp': inputCcs811ConstBaselineUpdate.checked,
        'hdcTemp': inputHdc1080TempSubaddress.value,
        'hdcHum': inputHdc1080HumiditySubaddress.value,
        'bmpTemp': inputHdcBmp280TempSubaddress.value,
        'bmpPres': inputHdcBmp280PressureSubaddress.value,
        'dsPin': parseInt(inputSensorDs18b20Pin.value),
        'dsTemp': inputDs18b20TempSubaddress.value
      };
  
      return sensorSettingsObj;
    }
  
  </script>
  
  <div class="setting-container">
    <label>CJMCU8128 SDA pin: </label>
    <input type="number" class="number-input" id="inputSensorSdaPin" disabled />
  </div>
  <div class="setting-container">
    <label>CJMCU8128 SCL pin: </label>
    <input type="number" class="number-input" id="inputSensorSclPin" disabled />
  </div>
  <div class="setting-container">
    <label>Scan devices at startup: </label>
    <input type="checkbox" id="inputActivateScanDevices" disabled />
  </div>
  <div class="setting-container">
    <label>CCS811 Baseline: </label>
    <input type="number" class="number-input" id="inputCcs811Baseline" disabled />
  </div>
  <div class="setting-container">
    <label>Constantly update CCS811 Baseline: </label>
    <input type="checkbox" id="inputCcs811ConstBaselineUpdate" disabled />
  </div>
  <div class="setting-container">
    <label>CCS811 CO2 subaddress (optional): </label>
    <input type="text" class="text-input" id="inputCcs811Co2Subaddress" disabled />
  </div>
  <div class="setting-container">
    <label>CCS811 TVOC subaddress (optional): </label>
    <input type="text" class="text-input" id="inputCcs811TvocSubaddress" disabled />
  </div>
  <div class="setting-container">
    <label>HDC1080 Temperature subaddress (optional): </label>
    <input type="text" class="text-input" id="inputHdc1080TempSubaddress" disabled />
  </div>
  <div class="setting-container">
    <label>HDC1080 Humidity subaddress (optional): </label>
    <input type="text" class="text-input" id="inputHdc1080HumiditySubaddress" disabled />
  </div>
  <div class="setting-container">
    <label>BMP280 Temperature subaddress (optional): </label>
    <input type="text" class="text-input" id="inputHdcBmp280TempSubaddress" disabled />
  </div>
  <div class="setting-container">
    <label>BMP280 Pressure subaddress (optional): </label>
    <input type="text" class="text-input" id="inputHdcBmp280PressureSubaddress" disabled />
  </div>
  <div class="setting-container">
    <label>DS18B20 data pin (optional): </label>
    <input type="number" class="number-input" id="inputSensorDs18b20Pin" disabled />
  </div>
  <div class="setting-container">
    <label>DS18B20 Temperature subaddress (optional): </label>
    <input type="text" class="text-input" id="inputDs18b20TempSubaddress" disabled />
  </div>
)=====";