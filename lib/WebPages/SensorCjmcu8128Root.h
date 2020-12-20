#include <Arduino.h>

const char sensorCjmcu8128Root[] PROGMEM = R"=====(
<script>
    var co2Display;
    var tvocDisplay;
    var tempHdcDisplay;
    var tempBmpDisplay;
    var humidityDisplay;
    var pressureDisplay;

    function sensorOnload() {
        co2Display = document.getElementById('co2Display');
        tvocDisplay = document.getElementById('tvocDisplay');
        tempHdcDisplay = document.getElementById('tempHdcDisplay');
        tempBmpDisplay = document.getElementById('tempBmpDisplay');
        humidityDisplay = document.getElementById('humidityDisplay');
        pressureDisplay = document.getElementById('pressureDisplay');
    }

    function displaySensorValues(values) {
        json = JSON.parse(values);

        if(json['error']) {
            console.log(json['error']);
            vocDisplay.textContent = 'Current VOC level: ' + 'NaN';
        } else {
            if (json['co2'])
                co2Display.textContent = 'Current CO2 level: ' + json['co2'] + ' ppm';
            if (json['tvoc'])
                tvocDisplay.textContent = 'Current TVOC level: ' + json['tvoc'] + ' ppb';
            if (json['tempBmp280'])
                tempBmpDisplay.textContent = 'Current Temperature (BMP280): ' + json['tempBmp280'] + ' \u00B0 C';
            if (json['tempHdc1080'])
                tempHdcDisplay.textContent = 'Current Temperature (HDC1080): ' + json['tempHdc1080'] + ' \u00B0 C';     
            if (json['humidity'])
                humidityDisplay.textContent = 'Current Humidity: ' + json['humidity'] + ' %';
            if (json['pressure'])
                pressureDisplay.textContent = 'Current Pressure: ' + json['pressure'] + ' hPa';
        }        
    }
</script>

<h2 id="co2Display"></h2>
<h2 id="tvocDisplay"></h2>
<h2 id="tempHdcDisplay"></h2>
<h2 id="tempBmpDisplay"></h2>
<h2 id="humidityDisplay"></h2>
<h2 id="pressureDisplay"></h2>
)=====";