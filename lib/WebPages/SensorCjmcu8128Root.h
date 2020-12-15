#include <Arduino.h>

const char sensorCjmcu8128Root[] PROGMEM = R"=====(
<script>
    var vocDisplay;

    function sensorOnload() {
        vocDisplay = document.getElementById('voc-display');
    }

    function displaySensorValues(values) {
        json = JSON.parse(values);

        if(json['error']) {
            console.log(json['error']);
            vocDisplay.textContent = 'Current VOC level: ' + 'NaN';
        } else if (json['voc']) {
            var vocLevel = json['voc'];
            vocDisplay.textContent = 'Current VOC level: ' + vocLevel;
        }
        
    }
</script>

<h2 id="voc-display"></h2>
)=====";