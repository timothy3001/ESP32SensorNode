#include <Arduino.h>

const char sensorThermometerRoot[] PROGMEM = R"=====(
<script>
    var tempDisplay;

    function sensorOnload() {
        tempDisplay = document.getElementById('temp-display');
    }

    function displaySensorValues(values) {
        json = JSON.parse(values);

        if(json['error']) {
            console.log(json['error']);
            tempDisplay.textContent = 'Current temperature: ' + 'NaN';
        } else if (json['temperature']) {
            var temp = Math.round(json['temperature'] * 10) / 10;
            tempDisplay.textContent = 'Current temperature: ' + temp + ' \u00B0C';
        }
        
    }
</script>

<h2 id="temp-display"></h2>
)=====";