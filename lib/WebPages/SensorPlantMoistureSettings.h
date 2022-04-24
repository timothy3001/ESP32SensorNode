#include <Arduino.h>

const char sensorPlantMoistureSettings[] PROGMEM = R"=====(
<script>
    var inputSensorPlantMoisturePin;
    var inputSensorMaxValue;
    var inputSensorMinValue;
    var inputSensorAbsoluteSubaddress;
    var inputSensorPercentageSubaddress;

    function sensorOnload() {
        inputSensorPlantMoisturePin = document.getElementById('inputSensorPlantMoisturePin');
        inputSensorMaxValue = document.getElementById('inputSensorMaxValue');
        inputSensorMinValue = document.getElementById('inputSensorMinValue');
        inputSensorAbsoluteSubaddress = document.getElementById('inputSensorAbsoluteSubaddress');
        inputSensorPercentageSubaddress = document.getElementById('inputSensorPercentageSubaddress');
    }

    function displayCurrentSensorSettings(settings) {
        inputSensorPlantMoisturePin.value = settings['pmPin'];
        inputSensorMaxValue.value = settings['pmMaxVal'];
        inputSensorMinValue.value = settings['pmMinVal'];
        inputSensorAbsoluteSubaddress.value = settings['pmAbsSubAddr'];
        inputSensorPercentageSubaddress.value = settings['pmPercSubAddr'];
    }

    function getSensorSettings() {
        if (inputSensorPlantMoisturePin.value < 0 || inputSensorPlantMoisturePin.value > 42) {
            window.alert('Please enter a valid pin for connection to moisture sensor!');
            return undefined;
        }

        if (inputSensorMaxValue.value < 0 || inputSensorMaxValue.value > 4095) {
            window.alert('Please enter a valid value for Max reading!');
            return undefined;
        }

        if (inputSensorMinValue.value < 0 || inputSensorMinValue.value > 4095) {
            window.alert('Please enter a valid value for Min reading!');
            return undefined;
        }

        if (inputSensorMinValue.value >= inputSensorMaxValue.value) {
            window.alert('Value for Max must be larger than value for Min!');
            return undefined;
        }

        var sensorSettingsObj = {
            'pmPin': parseInt(inputSensorPlantMoisturePin.value),
            'pmMaxVal': parseInt(inputSensorMaxValue.value),
            'pmMinVal': parseInt(inputSensorMinValue.value),
            'pmAbsSubAddr': inputSensorAbsoluteSubaddress.value,
            'pmPercSubAddr': inputSensorPercentageSubaddress.value,
        };

        return sensorSettingsObj;
    }
</script>

<div class="setting-container">
    <label>Plant moisture sensor pin: </label>
    <input type="number" class="number-input" id="inputSensorPlantMoisturePin" disabled />
</div>
<div class="setting-container">
    <label>Measure value "wet" (max): </label>
    <input type="number" class="number-input" id="inputSensorMaxValue" disabled />
</div>
<div class="setting-container">
    <label>Measure value "dry" (min): </label>
    <input type="number" class="number-input" id="inputSensorMinValue" disabled />
</div>
<div class="setting-container">
    <label>Moisture level "absolute" value subaddress (optional): </label>
    <input type="text" class="text-input" id="inputSensorAbsoluteSubaddress" disabled />
</div>
<div class="setting-container">
    <label>Moisture level "percentage" value subaddress (optional): </label>
    <input type="text" class="text-input" id="inputSensorPercentageSubaddress" disabled />
</div>
)=====";