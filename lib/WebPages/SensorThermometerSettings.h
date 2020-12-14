#include <Arduino.h>

const char sensorThermometerSettings[] PROGMEM = R"=====(
<script>
  var inputSensorPin;
  var inputSensorAddress;

  function sensorOnload() {
    inputSensorPin = document.getElementById('inputSensorPin');
    inputSensorAddress = document.getElementById('inputSensorAddress');
  }

  function displayCurrentSensorSettings(settings) {
    inputSensorPin.value = settings['dataPin'];
    inputSensorAddress.value = settings['tempAdd'];
  
    inputSensorPin.disabled = false;
    inputSensorAddress.disabled = false;
  }

  function getSensorSettings() {
    if (inputSensorPin.value < 0 && inputSensorPin > 42)
    {
      window.alert('Please enter a valid pin you will use for DS18B20 OneWire connection!');
      return undefined;
    }
    if (inputSensorAddress.value) {
      window.alert('Please enter a valid subaddress for the thermometer to send values to!');
    }

    var sensorSettingsObj = {
      'dataPin': parseInt(inputSensorPin.value),
      'tempAdd': inputSensorAddress.value
    };

    return sensorSettingsObj;
  }

</script>

<div class="setting-container">
  <label>DS18B20 OneWire Pin: </label>
  <input type="number" class="number-input" id="inputSensorPin" disabled />
</div>
<div class="setting-container">
  <label>Subaddress temperature value: </label>
  <input type="number" class="number-input" id="inputSensorAddress" disabled />
</div>
)=====";