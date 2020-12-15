#include <Arduino.h>

const char sensorCjmcu8128Settings[] PROGMEM = R"=====(
<script>
    var inputSensorSdaPin;
    var inputSensorSclPin;
    var inputActivateScanDevices;
  
    function sensorOnload() {
      inputSensorSdaPin = document.getElementById('inputSensorSdaPin');
      inputSensorSclPin = document.getElementById('inputSensorSclPin');
      inputActivateScanDevices = document.getElementById('inputActivateScanDevices');
    }
  
    function displayCurrentSensorSettings(settings) {
      inputSensorSdaPin.value = settings['sdaPin'];
      inputSensorSclPin.value = settings['sclPin'];
      inputActivateScanDevices.checked = settings['scanDevs'];
    
      inputSensorSdaPin.disabled = false;
      inputSensorSclPin.disabled = false;
      inputActivateScanDevices.disabled = false;
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
        'scanDevs': inputActivateScanDevices.checked
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
)=====";