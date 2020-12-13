#ifndef SensorBase_h_
#define SensorBase_h_

#include <Arduino.h>

/**
 * This is a base class which should be used to implement new sensor types.
 * 'getSensorInformationHtml()', 'getConfigurationPageHtml()', 'updateSettings()',
 * 'getValues()' and 'getSettings()' will be called by the webserver. 
 */

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
         * This function gets called by the webserver when the main page is loading.
         * It will be inserted into a certain part of the main page, see RootPage.html.
         * This function will only be called once and the returned string
         * should contain HTML code.
         * 
         * The root page contains some JavaScript code that is responsible for 
         * updating the current sensor values in the page. When the update is 
         * done it calls the function 'displaySensorValues(string values)'. 
         * You should implement this function in the HTML code to update the 
         * displaying when new sensor values arrived.
         * 
         * @return String that contains the sensor specific HTML code to be embedded.
         */
        virtual String getSensorInformationHtml();

        /**
         * This function gets called by the webserver when the settings page is loading.
         * it will be inserted into a certain part of the settings page, see 
         * SettingsPage.html.
         * 
         * The settings page has some JavaScript code which loads the current settings.
         * After loading of the settings the function 
         * 'displayCurrentSettings(string settings)' will be called containing current
         * sensor settings. You should implement this function to display the current
         * sensor settings in the settings page.
         * 
         * In addition to this you also must implement the javascript function 
         * 'getSensorSettings()' which should return the sensor specific settings the 
         * user entered into the fields. It needs to be formatted as JSON.
         * 
         * @return String that contains the sensor specific HTML code to be embedded.
         */
        virtual String getConfigurationPageHtml();

        /**
         * This function will be called by the webserver when the user clicks
         * 'Save'. The string 'settings' contains the JSON you set via 
         * 'getSensorSettings()' in the settings page.
         * 
         * @param settings String containing entered settings in the settings page
         */
        virtual void updateSettings(String settings);

        /**
         * Called by the webserver to retrieve the current sensor values. 
         * The root page will call the javascript function 
         * 'displaySensorValues(string values)'to display the current values. 
         * This function should be able to deliver the values every second.
         * Make sure, that you don't need too long to get the new sensor value.
         * 
         * Data should be formatted as JSON.
         * 
         * @return JSON as String containing needed values.
         */
        virtual String getValues();

        /**
         * Called by the webserver to retrieve the current sensor specific settings. 
         * These can be used to display the current settings in the web view. 
         * The javascript 'displayCurrentSettings(settings)' will be called. If that
         * function is implemented it will be given all current settings.
         * 
         * Data should be formatted as a single JSON object.
         * 
         * @return JSON as String containing current sensor settings.
         */ 
        virtual String getSettings();

        /** 
         * Called when reporting is active every time reporting will be done. Be aware 
         * that this function is called soon after bootup. The microcontroller resets 
         * afterwards.
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
         *  Return the name prefix for that sensor type.
         */
        virtual String getNamePrefix();
};

#endif