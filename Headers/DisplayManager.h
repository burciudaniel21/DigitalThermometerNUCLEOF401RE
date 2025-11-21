#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "LCD20x4.h"
#include <cstdio>

class DisplayManager {
private:
    LCD20x4 lcd;
    uint8_t degreeSymbol[8] = {
        0b00110,
        0b01001,
        0b01001,
        0b00110,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    };
public:
    DisplayManager() 
        : lcd(D8, D9, D10, D11, D12, D13) 
    {
        lcd.clear();
        lcd.createChar(0, degreeSymbol);

    }

    void ShowData(float current, float min, float max, const char* status) 
    {
        char buffer[21];

        lcd.clear();

        // Line 1: Current temperature
        lcd.setCursor(0, 0);
        snprintf(buffer, sizeof(buffer), "Now: %.2f ", current);
        lcd.print(buffer);
        lcd.writeChar(0);   // print the degree symbol
        lcd.print("C");

        // Line 2: Min
        lcd.setCursor(0, 1);
        snprintf(buffer, sizeof(buffer), "Min: %.2f ", min);
        lcd.print(buffer);
        lcd.writeChar(0);   // print the degree symbol
        lcd.print("C");

        // Line 3: Max
        lcd.setCursor(0, 2);
        snprintf(buffer, sizeof(buffer), "Max: %.2f ", max);
        lcd.print(buffer);
        lcd.writeChar(0);   // print the degree symbol
        lcd.print("C");

        // Line 4: Status
        lcd.setCursor(0, 3);
        lcd.print(status); 
    }
};

#endif
