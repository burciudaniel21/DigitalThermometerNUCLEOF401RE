#include "mbed.h"
#include "IntervalTimer.h"
#include "DS1631.h"
#include "TemperatureManager.h"
#include "AlertDevice.h"

//Create the DS1631 connection and object
    I2C i2c(D14, D15); //using I²C communication. SDA pin is D14, and SCL pin is D15.
    DS1631 sensor(i2c);
    AlertDevice warningLED(D2,true);
    AlertDevice greenLED(D4,true);


    IntervalTimer DS1631_Timer; //Create an instance of the Timer class
    
    TemperatureManager tempManager(sensor);

    
    void UpdateDevices() 
    {
        warningLED.Update();
        greenLED.Update();
    }

    int main() {
      sensor.Start();

      while (true) { // main program loop

        UpdateDevices();

        if (DS1631_Timer.HasPassed(500)) {    // every 500 ms
            float currentTemp = tempManager.GetTemperature();
            tempManager.UpdateMinMax(currentTemp);

            const char* status = tempManager.CheckStatus(currentTemp);

                  printf("Current: %.2f °C | Status: %s | Max: %.2f | Min: %.2f\r\n",
                   currentTemp, status,
                   tempManager.GetMaxTemp(), tempManager.GetMinTemp());

            if(strcmp(status, "TOO HOT") == 0){ //test code for turning the LED on.
                warningLED.TogglePattern(400,100);

            }
            else if(strcmp(status, "TOO COLD") == 0)
            {
                warningLED.ActivateFor();
            }
            else if (strcmp(status, "NORMAL") == 0) {
                greenLED.TogglePattern(1000, 400);
                warningLED.Off();
            }
            else{
                warningLED.Off();
                greenLED.Off();
            }
        }
    }
}    

