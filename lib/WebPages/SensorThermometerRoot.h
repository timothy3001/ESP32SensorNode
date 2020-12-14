#include <Arduino.h>

const char sensorThermometerRoot[] PROGMEM = R"=====(
<script>
    var tempDisplay;

    function sensorOnload() {
        tempDisplay = document.getElementById('temp-display');
    }

    function displaySensorValues(tempValue) {
        tempDisplay.textContent = 'Current temperature: ' + tempValue + '&deg;C';
    }
</script>

<h2 id="temp-display">Current temperature: NaN</h2>
)=====";