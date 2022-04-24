#include <Arduino.h>

const char sensorPlantMoistureRoot[] PROGMEM = R"=====(
<script>
    var plantMoistureAbsolute;
    var plantMoisturePercentage;

    function sensorOnload() {
        plantMoistureAbsolute = document.getElementById('plantMoistureAbsolute');
        plantMoisturePercentage = document.getElementById('plantMoisturePercentage');
    }

    function displaySensorValues(values) {
        json = JSON.parse(values);

        if (json['error']) {
            console.log(json['error']);
            plantMoistureAbsolute.textContent = 'Plant moisture level (absolute): ' + 'NaN';
        } else {
            if (json['plantMoistAbs'] != null)
                plantMoistureAbsolute.textContent = 'Plant moisture level (absolute): ' + json['plantMoistAbs'];
            if (json['plantMoistPerc'] != null)
                plantMoisturePercentage.textContent = 'Plant moisture level (percentage): ' + json['plantMoistPerc'] + ' %';
        }        
    }
</script>

<h2 id="plantMoistureAbsolute"></h2>
<h2 id="plantMoisturePercentage"></h2>
)=====";