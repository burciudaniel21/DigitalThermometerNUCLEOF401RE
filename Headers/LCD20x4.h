#ifndef LCD20X4_H
#define LCD20X4_H

#include "mbed.h"

class LCD20x4 {
private:
    // LCD control pins
    DigitalOut rs;      // Register Select: 0 = command, 1 = data
    DigitalOut en;      // Enable: rising edge triggers data latch

    // LCD data pins (4-bit mode uses only D4–D7)
    DigitalOut d4, d5, d6, d7;

    /**
     * Pulses the Enable (EN) pin to instruct the LCD to read the data lines.
     * The timing is critical according to the HD44780 controller specification.
     */
    void pulseEnable() {
        en = 0;
        wait_us(1);     // Small stabilisation delay
        en = 1;         // Rising edge latches the data
        wait_us(1);
        en = 0;         // Return to low
        wait_us(100);   // Allow LCD time to process
    }

    /**
     * Sends a single 4-bit group (nibble) to the LCD.
     * The LCD expects data on pins D4–D7, then a pulse on EN.
     */
    void write4Bits(uint8_t value) {
        d4 = (value >> 0) & 0x01;
        d5 = (value >> 1) & 0x01;
        d6 = (value >> 2) & 0x01;
        d7 = (value >> 3) & 0x01;
        pulseEnable();
    }

    /**
     * Sends an entire 8-bit value in two 4-bit transmissions.
     * @param value 8-bit data or command
     * @param mode 0 for command, 1 for character data
     */
    void send(uint8_t value, bool mode) {
        rs = mode;              // Set data/command mode
        write4Bits(value >> 4); // Send upper nibble first
        write4Bits(value & 0x0F); // Then lower nibble
    }

    /**
     * Sends a command to the LCD and allows time for internal processing.
     */
    void command(uint8_t value) {
        send(value, 0);         // 0 = command mode
        ThisThread::sleep_for(2ms);
    }

public:
    /**
     * Constructor: initialises the LCD pins and runs the mandatory
     * 4-bit initialisation sequence defined by the HD44780 datasheet.
     */
    LCD20x4(PinName rsPin, PinName enPin,
            PinName d4Pin, PinName d5Pin,
            PinName d6Pin, PinName d7Pin)
        : rs(rsPin), en(enPin),
          d4(d4Pin), d5(d5Pin),
          d6(d6Pin), d7(d7Pin)
    {
        en = 0;
        ThisThread::sleep_for(50ms);   // Allow LCD power-up stabilisation

        // Initialise in 4-bit mode (specific sequence required)
        write4Bits(0x03);
        ThisThread::sleep_for(5ms);

        write4Bits(0x03);
        wait_us(150);

        write4Bits(0x03);
        write4Bits(0x02);  // Command to switch to 4-bit mode

        // Function set: 4-bit, 2 lines, 5x8 characters
        command(0x28);

        // Display control: display ON, cursor OFF, blink OFF
        command(0x0C);

        // Entry mode: increment cursor, no display shift
        command(0x06);

        // Clear display
        clear();
    }

    /**
     * Clears the entire LCD display and resets cursor to home position.
     */
    void clear() {
        command(0x01);
        ThisThread::sleep_for(2ms); // Clearing requires extra delay
    }

    /**
     * Sets the cursor to a specific column and row.
     * The row offsets correspond to the internal DDRAM layout of 20x4 modules.
     */
    void setCursor(int col, int row) {
        static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
        command(0x80 | (col + row_offsets[row])); // Set DDRAM address
    }

    /**
     * Prints a null-terminated string to the LCD.
     * Each character is sent in data mode.
     */
    void print(const char* text) {
        while (*text) {
            send(*text, 1); // 1 = data mode
            text++;
        }
    }

        /**
     * Creates a custom 5x8 character in CGRAM.
     * index must be 0–7. pattern is an array of 8 bytes (each 5 bits used).
     */
    void createChar(uint8_t index, const uint8_t pattern[8]) {
        index &= 0x07;              // Only 0–7 allowed
        command(0x40 | (index << 3));  // Set CGRAM address

        for (int i = 0; i < 8; i++) {
            send(pattern[i], 1);    // Write each row of the character
        }
    }

        void writeChar(uint8_t index) {
        send(index, 1);  // 1 = data mode
    }

};


#endif
