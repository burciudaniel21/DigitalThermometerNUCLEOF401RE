#ifndef DS1631_H
#define DS1631_H

#include "mbed.h"

class DS1631{
    private:
        I2C &i2c;
        const int address;

    public:
        // Constructor
        DS1631(I2C &i2cBus, int addr = 0x90) : i2c(i2cBus), address(addr) {} //the 8 bit address has been obtained by reviewing the data sheet and converting the binary to hex

        // Start continuous temperature conversion
        void Start() 
        {
            // Tell the sensor to start continuous measurement. This command can be found in the datasheet
            char cmd = 0x51;
            i2c.write(address, &cmd, 1);
        }
        

        // Read temperature from DS1631
        float ReadTemperature() 
        {
            // Ask for temperature. This command can be found in the datasheet
            char cmd = 0xAA;
            i2c.write(address, &cmd, 1);

            // Read 2 bytes
            char data[2];
            i2c.read(address, data, 2);

            // Convert
            int16_t raw = (data[0] << 8) | data[1]; // Combine the two bits together. data[0] << 8 means “move it 8 bits to the left,” so it becomes the top half. | data[1] means “combine it with the bottom half." e.g. 00011001 00010000 which in decimal is 6416

            // Convert to Celsius
            return raw / 256.0f;
        }
};

#endif
