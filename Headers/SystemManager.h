#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "mbed.h"
#include "DS1631.h"
#include "TemperatureManager.h"
#include "AlertDevice.h"
#include "IntervalTimer.h"
#include "ToneDevice.h"

class SystemManager {
private:
    I2C i2c;
    DS1631 sensor;
    TemperatureManager tempManager;
    AlertDevice warningLED;
    AlertDevice greenLED;
    ToneDevice mainBuzzer;
    IntervalTimer timer;

public:
    SystemManager()
        : i2c(D14, D15),
          sensor(i2c),
          tempManager(sensor),
          warningLED(D2, true),
          greenLED(D4, true),
          mainBuzzer(D3)
          {}

    void UpdateDevices()
    {
        warningLED.Update();
        greenLED.Update();
    }

    void Run() {
        sensor.Start();

        while (true) 
        {
            UpdateDevices();

            if (timer.HasPassed(500)) {
                float currentTemp = tempManager.GetTemperature();
                tempManager.UpdateMinMax(currentTemp);
                const char* status = tempManager.CheckStatus(currentTemp);

                printf("Temp: %.2f °C | Status: %s | Max: %.2f | Min: %.2f\r\n",
                       currentTemp, status,
                       tempManager.GetMaxTemp(), tempManager.GetMinTemp());

                if (strcmp(status, "TOO HOT") == 0) 
                {
                    warningLED.TogglePattern(400, 100);
                    mainBuzzer.Beep(4000.F);


                } 
                else if (strcmp(status, "TOO COLD") == 0) 
                {
                    warningLED.ActivateFor();
                    mainBuzzer.Beep(2000.F);
                } 
                else if (strcmp(status, "NORMAL") == 0) 
                {
                    greenLED.TogglePattern(1000, 400);
                    warningLED.Off();
                    mainBuzzer.Off();
                } 
                else 
                {
                    warningLED.Off();
                    greenLED.Off();
                }
            }
        }
    }
};

#endif
