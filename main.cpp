#include "SystemManager.h"
#include "LCD20x4.h"

int main() {
    
LCD20x4 lcd(D8, D9, D10, D11, D12, D13);
    lcd.print("Hello World!");
    lcd.setCursor(0, 1);
    lcd.print("LCD Working :)");
    lcd.setCursor(0, 2);
    lcd.print("Line 3 OK!");
    lcd.setCursor(0, 3);
    lcd.print("20x4 Display Ready!");
    
    SystemManager system;
    system.Run();
    
}
