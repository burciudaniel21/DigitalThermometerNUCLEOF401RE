#ifndef TEMPERATUREMANAGER_H
#define TEMPERATUREMANAGER_H

#include "mbed.h"
#include "DS1631.h"

class TemperatureManager 
{
private:
    DS1631 &sensor;
    float upperTempLimit;
    float lowerTempLimit;
    float maxTempRecorded;
    float minTempRecorded;

public:
    TemperatureManager(DS1631 &sensorRef, float minTemp = 24.0f, float maxTemp = 26.5f)
        : sensor(sensorRef), lowerTempLimit(minTemp), upperTempLimit(maxTemp),
          maxTempRecorded(-1000), minTempRecorded(1000) //initialisation values (impossible to reach)
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
        if (temp >= upperTempLimit) return "TOO HOT";
        if (temp < lowerTempLimit) return "TOO COLD";
        return "NORMAL";
    }

    void UpdateMinMax(float temp) 
    {
        if (temp > maxTempRecorded) maxTempRecorded = temp;
        if (temp < minTempRecorded) minTempRecorded = temp;
    }

    float GetMaxTemp() const { return maxTempRecorded; }
    float GetMinTemp() const { return minTempRecorded; }

    void ResetMinMax() 
    {
        maxTempRecorded = -1000;
        minTempRecorded = 1000;
    }
};
#endif

