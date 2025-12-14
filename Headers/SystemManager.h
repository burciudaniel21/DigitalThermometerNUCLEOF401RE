#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "mbed.h"
#include "DS1631.h"
#include "TemperatureManager.h"
#include "AlertDevice.h"
#include "IntervalTimer.h"
#include "ToneDevice.h"
#include "DisplayManager.h"
#include <cstdio>

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
    IntervalTimer hourlyResetTimer;
    DigitalIn btnUp;
    DigitalIn btnDown;
    DigitalIn btnSelect;
    BufferedSerial loggerSerial;
    IntervalTimer logTimer;

    float typeASound = 4000.F;
    float typeBSound = 2000.F;

    static const int loggerBaud = 9600;

public:
    SystemManager()
        : i2c(D14, D15),
          sensor(i2c),
          tempManager(sensor),
          warningLED(D2, true),
          greenLED(D4, true),
          mainBuzzer(D3),
          btnUp(D6, PullUp),
          btnDown(D7, PullUp),
          btnSelect(D5, PullUp),
          loggerSerial(D1, D0, loggerBaud)   // TX, RX, baud
          {}

    void UpdateDevices()
    {
        warningLED.Update();
        greenLED.Update();
    }

    void Run() 
    {
        InitialSetup();

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

               /* printf("Temp: %.2f °C | Status: %s | Max: %.2f | Min: %.2f\r\n",
                    currentTemp, status,
                    tempManager.GetMaxTemp(), tempManager.GetMinTemp()); */

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
                    tempManager.GetLowerThreshold(),
                    tempManager.GetUpperThreshold(),
                    status
                );
            }
                        
            if (hourlyResetTimer.HasPassed(3600000)) // 1 hour
            { 
                tempManager.ResetMinMax();
            }

            if (logTimer.HasPassed(1000)) //send data every 1 sec
            {
                LogToExternal(currentTemp, tempManager.GetMinTemp(), tempManager.GetMaxTemp(), status);
            }
            
        }
    }

    void InitialSetup()
    {
        float lower = tempManager.GetLowerThreshold();
        float upper = tempManager.GetUpperThreshold();

        bool settingLower = true;
        bool settingUpper = false;
        bool updateDisplay = true;

        bool setupComplete = false;
        char buffer[21];

        IntervalTimer debounceTimer;   // prevents button spam
        const int debounceDelay = 200; // between accepted presses

        while (!setupComplete)
        {
            if (updateDisplay)
            {
                if (settingLower)
                {
                    snprintf(buffer, sizeof(buffer), "Min Temp: %.0f", lower);
                }
                else
                {
                    snprintf(buffer, sizeof(buffer), "Max Temp: %.0f", upper);
                }

                display.PrintDataOnRow(0, buffer);
                display.PrintCelsiusSymbol();
                display.PrintDataOnRow(1, "UP     +1");
                display.PrintCelsiusSymbol();
                display.PrintDataOnRow(2, "DOWN   -1");
                display.PrintCelsiusSymbol();
                display.PrintDataOnRow(3, "SELECT to continue");
                updateDisplay = false;
            }

            // Button presses are accepted only when debounce timer expires
            if (debounceTimer.HasPassed(debounceDelay))
            {                                
                if (btnUp.read() == 0)
                {
                    if (settingLower)
                    {
                        if (lower < upper - 1)
                            lower++;
                    }
                    else
                    {
                        upper++;
                    }
                    updateDisplay = true;
                    debounceTimer.Reset();
                }

                if (btnDown.read() == 0)
                {
                    if (settingLower)
                    {
                        lower--;
                    }
                    else if (upper > lower + 1)
                    {
                        upper--;
                    }
                    updateDisplay = true;
                    debounceTimer.Reset();
                }

                if (btnSelect.read() == 0)
                {
                    if (settingLower)
                    {
                        settingLower = false;
                        settingUpper = true;
                    }
                    else // final confirmation
                    {
                        tempManager.SetThresholds(lower, upper);
                        setupComplete = true;
                    }
                    updateDisplay = true;
                    debounceTimer.Reset();
                }
            }
        }
    }

    void LogToExternal(float temp, float minT, float maxT, const char* status)
    { //used to send data to Arduino (or other device)
        char buffer[80];

        int len = snprintf(
            buffer,
            sizeof(buffer),
            "LOG,TEMP=%.2f,MIN=%.2f,MAX=%.2f,STATUS=%s\r\n",
            temp, minT, maxT, status
        );

        loggerSerial.write(buffer, len);
    }
};

#endif
