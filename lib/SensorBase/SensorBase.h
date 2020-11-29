class SensorBase
{
    public:
        virtual void resetSettings();
        virtual void begin();
        virtual String getSensorInformationHtml();
        virtual String getConfigurationPageHtml();
        virtual void updateSettings();
};