# Digital Thermometer System (STM32 NUCLEO-F401RE, C++, mbed)

An embedded digital thermometer system for the STM32 NUCLEO-F401RE built in C++ using mbed OS.

This project reads temperature from a DS1631 sensor over I2C, displays live and historical values on a 20x4 LCD, allows the user to configure temperature thresholds using on-board buttons, triggers visual and audible alerts, and streams logs over serial to an external device.
---

## Key Features

- **Temperature sensing (I2C)**
  - DS1631 sensor driver and continuous conversion mode
  - Regular polling and conversion to Celsius

- **Live display (20x4 LCD)**
  - Custom HD44780-style LCD driver (4-bit parallel)
  - Custom degree symbol for Celsius output
  - Displays:
    - Min recorded
    - Max recorded
    - Current temperature
    - System status (NORMAL, TOO HOT, TOO COLD)

- **User-configurable thresholds**
  - Setup flow to set min and max thresholds
  - Buttons: UP (+1), DOWN (-1), SELECT (continue/confirm)
  - Debounce timer to prevent button spam

- **Alert system**
  - Warning LED patterns when TOO HOT / TOO COLD
  - Green LED pulse pattern during NORMAL operation
  - PWM buzzer tones with different frequencies by alert type

- **Min/Max tracking and timed reset**
  - Tracks min and max recorded temperatures during runtime
  - Automatically resets min and max every hour

- **Serial logging**
  - Sends temperature, min, max, and status every second over serial
  - Log format example:
    `LOG,TEMP=...,MIN=...,MAX=...,STATUS=...`

- **Non-blocking timing**
  - Uses interval timers for periodic tasks (sensor read, display refresh, hourly reset, serial logging)
  - Main loop stays responsive and does not rely on long blocking delays

---

## Hardware

- STM32 NUCLEO-F401RE (ARM Cortex-M4)
- DS1631 temperature sensor (I2C)
- 20x4 character LCD (HD44780 compatible, 4-bit mode)
- LEDs (status and warning)
- Buzzer (PWM capable)
- 3 push buttons (UP, DOWN, SELECT)

---

## Build and Run

1. Clone the repository
2. Open in your mbed IDE
3. Ensure all of the sensors are correctly connected and the pins are assigned
4. Build for NUCLEO_F401RE and flash to the board.

---
   
**Project Structure (high level)**

-main.cpp
  - Creates SystemManager and runs the system loop
- Headers/SystemManager.h
  - Controls sensors, display, alerts, user input, timers, and logging
- Headers/TemperatureManager.h
  - Temperature reads, min/max tracking, threshold checks
- Headers/DisplayManager.h, Headers/LCD20x4.h
  - LCD output and custom character support
- Headers/AlertDevice.h, Headers/ToneDevice.h
  - LED alert patterns and PWM buzzer tone output
- Headers/IntervalTimer.h
  -  Non-blocking interval logic for periodic tasks
-  Headers/DS1631.h
  -  DS1631 I2C driver

---

Author
Dan Burciu
