#include "mbed.h"
#include "DS1631.h"

class TemperatureManager 
{
private:
    DS1631 &sensor;
    float upperTempLimit;
    float lowerTempLimit;
    float maxTemp;
    float minTemp;

public:
    TemperatureManager(DS1631 &sensorRef, float minTemp = 20.0f, float maxTemp = 28.0f)
        : sensor(sensorRef), lowerTempLimit(minTemp), upperTempLimit(maxTemp),
          maxTemp(-1000), minTemp(1000) //initialisation values (impossible to reach)
          {}

    void SetThresholds(float lower, float upper) 
    {
        lowerTempLimit = lower;
        upperTempLimit = upper;
    }

    float GetTemperature() 
    {
        return sensor.ReadTemperature();
    }

    const char* CheckStatus(float temp) //char* returns a pointer to one of those three fixed messages in memory.
    {
        if (temp > upperTempLimit) return "TOO HOT";
        if (temp < lowerTempLimit) return "TOO COLD";
        return "NORMAL";
    }

    void UpdateMinMax(float temp) 
    {
        if (temp > maxTemp) maxTemp = temp;
        if (temp < minTemp) minTemp = temp;
    }

    float GetMaxTemp() const { return maxTemp; }
    float GetMinTemp() const { return minTemp; }

    void ResetMinMax() 
    {
        maxTemp = -1000;
        minTemp = 1000;
    }
};
