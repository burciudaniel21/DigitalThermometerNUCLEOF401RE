#ifndef ALERTDEVICE_H
#define ALERTDEVICE_H

#include "mbed.h"
#include "IntervalTimer.h"

// Generic digital output alert device (LED, buzzer, relay, etc.)
class AlertDevice {
private:
    DigitalOut device;
    bool activeHigh; //is the device an active high device?
    IntervalTimer timer; // a timer to control timed events

    bool active = false; //is the device currently active?
    bool toggling = false; //do we want the device to toggle between active and inactive?

    int onDuration = 0;
    int offDuration = 0;

public:
    AlertDevice(PinName pin, bool activeHighLogic = true)
        : device(pin), activeHigh(activeHighLogic) {
        Off(); // start in off state
    }

    // Turn the device on immediately
    void On() 
    {
        if(activeHigh) //this is required to ensure that all of the devices work with this class, not just activeHigh devices
        {
            device = 1;
        }
        else
        {
            device = 0;
        }
        active = true;
        toggling = false;
    }

    // Turn the device off immediately
    void Off() 
    {
        if(activeHigh) //this is required to ensure that all of the devices work with this class, not just activeHigh devices
        {
            device = 0;
        }
        else
        {
            device = 1;
        }        
        active = false;
        toggling = false;
    }

    // Turn the device on for a set duration (or indefinitely if 0)
    void ActivateFor(int durationMs = 0) 
    {
        On();
        if (durationMs > 0) {
            onDuration = durationMs;
            timer.Reset();
        } else 
        {
            onDuration = 0; // no timing limit
        }
    }

    // Start continuous toggle mode e.g. TogglePattern(200, 800) - ON 200ms, OFF 800ms, repeat
    void TogglePattern(int onTimeMs, int offTimeMs) 
    {
        onDuration = onTimeMs;
        offDuration = offTimeMs;
        toggling = true;
        //timer.Reset();
        //On();
    }

    // Must be called frequently in main loop
    void Update() 
    {
        // Handle single timed activation
    if (active && !toggling && onDuration > 0 && timer.HasPassed(onDuration)) 
    {
        Off();
    }

    // Handle toggle pattern
    else if (toggling) 
    {
        if (active) 
        {
            // currently ON — check if ON time has expired
            if (timer.HasPassed(onDuration)) {
                Off();
                timer.Reset();
            }
        } 
        else 
        {
            // currently OFF — check if OFF time has expired
            if (timer.HasPassed(offDuration)) {
                On();
                timer.Reset();
            }
        }
    }
    }
};

#endif
