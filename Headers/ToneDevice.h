#include "mbed.h"

class ToneDevice {
private:
    PwmOut pwm;     // Used to generate a square wave (tone)
    bool active;    // Keeps track of whether the buzzer is currently sounding

public:
    // Constructor: called when you create a BuzzerDevice object
    ToneDevice(PinName pin) : pwm(pin), active(false) {
        pwm.write(0.0f); // Ensure the buzzer is silent when created
    }

    // Turns the buzzer ON with a specified frequency and duty cycle
    void Beep(float frequencyHz = 4000.0f, float duty = 0.5f) {
        pwm.period(1.0f / frequencyHz); // Set tone frequency (period = 1/f)
        pwm.write(duty);                // Set duty cycle (how much time pin is HIGH)
        active = true;                  // Update internal state
    }

    // Turns the buzzer OFF
    void Off() {
        pwm.write(0.0f); // Stop PWM signal (pin always LOW)
        active = false;  // Update internal state
    }

    // Returns whether the buzzer is currently active
    bool IsActive() const {
        return active;
    }
};
