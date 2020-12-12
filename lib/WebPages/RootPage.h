#include <Arduino.h>

const char rootPage[] PROGMEM = R"=====(
  <html>

  <head>
    <style>
      #base-app {
        width: 100%%;
        height: 100%%;
        display: flex;
        flex-direction: column;
      }

      header {
        width: 100%%;
        display: flex;
        flex-direction: row;
        justify-content: space-between;
      }

      html {
        background-color:#2D333C;
        margin: 0;
        padding: 0;        
      }

      body {
        color: #DDDDDD;
        font-family: Arial, Helvetica, sans-serif;
        margin: 20px;
        padding: 0;
      }

      hr {
        width: 100%%;
        height: 1.5px;
        background-color: #DDDDDD;
        border: 0;
        stroke-width: 5px;
      }

      .settings-button {
        width: 35px;
        height: 35px;
        align-self: center;
        flex-basis: auto;


      }

      #sensor-name {
        flex-basis: content;
      }

    </style>

    <script>
      window.onload = () => {
        window.setTimeout(update, 1000);
      }

      function update() {
        var xlr = new XMLHttpRequest();
        xlr.open('GET', '/api/values', false);
        xlr.send(null);
        
        xlr.onreadystatechange = () => {
          if (this.readystate == 4 && this.status == 200) {
            if (typeof updateValues == "function") {
              updateValues(this.responsetext);
            }
          }
        }

        window.setTimeout(update, 1000);
      }
    </script>        
  </head>
  <body>
    <div id="base-app">
      <header>
        <div>
          <h1 id="sensor-name">SensorNode</h1>
        </div>
        <a class="settings-button" href="/settings">
          <svg viewBox='0 0 24 24' width='35px' height='35px'><path fill='#8890AA' d='M19.14 12.94c.04-.3.06-.61.06-.94 0-.32-.02-.64-.07-.94l2.03-1.58c.18-.14.23-.41.12-.61l-1.92-3.32c-.12-.22-.37-.29-.59-.22l-2.39.96c-.5-.38-1.03-.7-1.62-.94l-.36-2.54c-.04-.24-.24-.41-.48-.41h-3.84c-.24 0-.43.17-.47.41l-.36 2.54c-.59.24-1.13.57-1.62.94l-2.39-.96c-.22-.08-.47 0-.59.22L2.74 8.87c-.12.21-.08.47.12.61l2.03 1.58c-.05.3-.09.63-.09.94s.02.64.07.94l-2.03 1.58c-.18.14-.23.41-.12.61l1.92 3.32c.12.22.37.29.59.22l2.39-.96c.5.38 1.03.7 1.62.94l.36 2.54c.05.24.24.41.48.41h3.84c.24 0 .44-.17.47-.41l.36-2.54c.59-.24 1.13-.56 1.62-.94l2.39.96c.22.08.47 0 .59-.22l1.92-3.32c.12-.22.07-.47-.12-.61l-2.01-1.58zM12 15.6c-1.98 0-3.6-1.62-3.6-3.6s1.62-3.6 3.6-3.6 3.6 1.62 3.6 3.6-1.62 3.6-3.6 3.6z'></path></svg>
        </a>
      </header>
      <hr />
      <main>
        <!-- Sensor specific content here -->
        %s
        <!-- End of sensor specific content -->
      </main>
    </div>
  </body>

  </html>
)=====";