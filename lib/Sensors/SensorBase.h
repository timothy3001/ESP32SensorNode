#ifndef SensorBase_h_
#define SensorBase_h_

class SensorBase
{
    public:
        /**
         * When called, all sesor specific settings should be resetted to the default. 
         */
        virtual void resetSettings();

        /**
         * Called at startup. You can use it for initialization.
         * Prefer to do initialization for reading out sensor values in this function
         * and not in the constructor.
         * 
         * It is recommended to get the first reading of the sensor here.
         */
        virtual void begin();

        /**
         * Called by the webserver to get the sensor specific HTML code for showing
         * sensor values. The HTML code is embedded into a special DIV-Element on 
         * the root page.
         * 
         * @return String that contains the sensor specific HTML code to be embedded.
         */
        virtual String getSensorInformationHtml();

        /**
         * Called by the webserver to get the sensor specific HTML code for showing
         * and manipulating settings. The HTML code is embedded into a special 
         * DIV-Element on the settings page.
         * 
         * @return String that contains the sensor specific HTML code to be embedded.
         */
        virtual String getConfigurationPageHtml();

        /**
         * Called by the webserver when settings are updated. 
         * 
         * @param settings JSON string containing the settings object.
         */
        virtual void updateSettings(String settings);

        /**
         * Called by the webserver to retrieve the current sensor values. 
         * These will call the javascript function 'updateValues(values)' to 
         * display the current values. This function should be able to deliver 
         * the values every second. Make sure, that you don't need too long to 
         * get the new sensor value.
         * 
         * Data should be formatted as JSON.
         * 
         * @return JSON as String containing needed values.
         */
        virtual String getValues();

        /**
         * Called by the webserver to retrieve the current sensor specific settings. These can be
         * used to display the current settings in the web view. The javascript 
         * 'setCurrentSettings(settings)' will be called. If that function is
         * implemented it will be given all current settings.
         * 
         * Data should be formatted as a single JSON object.
         * 
         * @return JSON as String containing current sensor settings.
         */ 
        virtual String getSettings();

        /** 
         * Called when reporting is active every time reporting will be done. Be aware that this 
         * function is called soon after bootup. The microcontroller resets afterwards.
         * 
         * @param baseAddress The base address of the device / server the values should be send to.
         */
        virtual void executeReporting(String baseAddress);

        /**
         * This function is called very regularly and often. You can use it to asynchronously 
         * update values, which are retrieved by the webserver when the web page is visited.
         */
        virtual void loop();

        /**
         *  
         */
        virtual String getNamePrefix();
};

#endif