#include "mbed.h"

class IntervalTimer {
private:
    Timer timer;      // Mbed timer to measure elapsed time
    int lastTime;     // Time (ms) when the interval last triggered

public:
    IntervalTimer() 
    { // Constructor
        timer.start();
        lastTime = 0;
    }

    // Returns true if the given interval (in ms) has passed
    bool HasPassed(int intervalMs) 
    {
        int now = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
        if (now - lastTime >= intervalMs) {
            lastTime = now;
            return true;
        }
        return false;
    }

    // Reset the timer manually
    void Reset() 
    {
        timer.reset();
        lastTime = 0;
    }
};