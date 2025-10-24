#include "mbed.h"
#include "IntervalTimer.h"
#include "DS1631.h"
#include "TemperatureManager.h"

//Create the DS1631 connection and object
    I2C i2c(D14, D15); //using I²C communication. SDA pin is D14, and SCL pin is D15.
    DS1631 sensor(i2c);

    IntervalTimer DS1631_Timer; //Create an instance of the Timer class
    
    TemperatureManager tempManager(sensor);

int main() {
    sensor.Start();

    while(true){ // main program loop
        if (DS1631_Timer.HasPassed(500)) {    // every 500 ms
            float currentTemp = tempManager.GetTemperature();
            tempManager.UpdateMinMax(currentTemp);

            const char* status = tempManager.CheckStatus(currentTemp);

                    printf("Current: %.2f °C | Status: %s | Max: %.2f | Min: %.2f\r\n",
                   currentTemp, status,
                   tempManager.GetMaxTemp(), tempManager.GetMinTemp());
        }
    }
}
