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

    void ShowData(float current, float min, float max, float lower, float upper, const char* status) 
    {
        char buf[21];
        lcd.clear();



        // --- Line 1 & 2: Min and Max ---
        lcd.setCursor(0, 0);
        snprintf(buf, sizeof(buf), "Min recorded: %.1f", min);
        lcd.print(buf);
        PrintCelsiusSymbol();
        lcd.print("C");

        lcd.setCursor(0, 1);
        snprintf(buf, sizeof(buf), "Max recorded: %.1f", max);
        lcd.print(buf);
        PrintCelsiusSymbol();
        lcd.print("C");

        // --- Line 3: Current temperature ---
        lcd.setCursor(0, 2);
        snprintf(buf, sizeof(buf), "Current temp: %.1f", current);
        lcd.print(buf);
        PrintCelsiusSymbol();
        lcd.print("C");

        // --- Line 3: User thresholds ---
        /*lcd.setCursor(0, 2);
        snprintf(buf, sizeof(buf), "Lo:%.1f", lower);
        lcd.print(buf);
        lcd.writeChar(0);
        lcd.print("C");

        lcd.setCursor(10, 2);
        snprintf(buf, sizeof(buf), "Hi:%.1f", upper);
        lcd.print(buf);
        lcd.writeChar(0);
        lcd.print("C");*/

        // --- Line 4: Status ---
        lcd.setCursor(0, 3);
        lcd.print("Status: ");
        lcd.print(status);
    }

    void PrintDataOnRow(int row, const char* text)
    {
        if(row < 4 && text != nullptr)
        {
            lcd.setCursor(0, row);
            lcd.print("                    ");
            lcd.setCursor(0, row);
            lcd.print(text);
        }
    }  

    void PrintCelsiusSymbol()
    {
        lcd.writeChar(0);
    }
};

#endif
