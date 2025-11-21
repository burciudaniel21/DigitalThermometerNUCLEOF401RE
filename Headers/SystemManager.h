#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "mbed.h"
#include "DS1631.h"
#include "TemperatureManager.h"
#include "AlertDevice.h"
#include "IntervalTimer.h"
#include "ToneDevice.h"
#include "DisplayManager.h"

class SystemManager {
private:
    I2C i2c;
    DS1631 sensor;
    TemperatureManager tempManager;
    AlertDevice warningLED;
    AlertDevice greenLED;
    ToneDevice mainBuzzer;
    IntervalTimer timer;
    DisplayManager display;
    IntervalTimer displayTimer;

    float typeASound = 4000.F;
    float typeBSound = 2000.F;

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

        float currentTemp = 0.0f;
        const char* status = "NORMAL";
        
        while (true) 
        {
            UpdateDevices();   // keeps LEDs and buzzer running
            
            if (timer.HasPassed(500)) {
                currentTemp = tempManager.GetTemperature();
                tempManager.UpdateMinMax(currentTemp);
                status = tempManager.CheckStatus(currentTemp);

                printf("Temp: %.2f °C | Status: %s | Max: %.2f | Min: %.2f\r\n",
                    currentTemp, status,
                    tempManager.GetMaxTemp(), tempManager.GetMinTemp());

                if (strcmp(status, "TOO HOT") == 0) 
                {
                    warningLED.TogglePattern(400, 100);
                    mainBuzzer.Beep(typeASound);
                } 
                else if (strcmp(status, "TOO COLD") == 0) 
                {
                    warningLED.ActivateFor();
                    mainBuzzer.Beep(typeBSound);
                } 
                else  // NORMAL
                {
                    greenLED.TogglePattern(1000, 400);
                    warningLED.Off();
                    mainBuzzer.Off();
                }
            }

            // --- LCD update (NON-BLOCKING) ---
            if (displayTimer.HasPassed(500)) {
                display.ShowData(
                    currentTemp,
                    tempManager.GetMinTemp(),
                    tempManager.GetMaxTemp(),
                    status
                );
            }
        }
    }
};

#endif
