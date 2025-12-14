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
        //lcd.clear();
        lcd.createChar(0, degreeSymbol);

    }

    void ShowData(float current, float min, float max, const char* status) 
    {
        char buf[21];
        //lcd.clear();

        // --- Line 1 & 2: Min and Max ---
        snprintf(buf, sizeof(buf), "Min recorded: %.1f", min);
        PrintDataOnRow(0, buf);

        PrintCelsiusSymbol();
        lcd.print("C");

        snprintf(buf, sizeof(buf), "Max recorded: %.1f", max);
        PrintDataOnRow(1, buf);
        PrintCelsiusSymbol();
        lcd.print("C");

        // --- Line 3: Current temperature ---
        snprintf(buf, sizeof(buf), "Current temp: %.1f", current);
        PrintDataOnRow(2, buf);
        PrintCelsiusSymbol();
        lcd.print("C");

        // --- Line 4: Status ---
        snprintf(buf, sizeof(buf), "Status: %.10s", status);
        PrintDataOnRow(3, buf);
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
