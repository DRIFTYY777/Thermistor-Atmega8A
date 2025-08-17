# Thermistor Temperature Display Project 🌡️

## Overview

This project uses an **Atmega8A** microcontroller to measure temperature using a **10k thermistor**. The thermistor is connected to the **A2 pin** through a **10k voltage divider**. The **analog-to-digital converter (ADC)** of the Atmega8A reads the voltage, converts it to temperature, and displays it on a **0.92" OLED display**.

## Hardware Setup ⚙️

* **Microcontroller:** Atmega8A
* **Thermistor:** 10k
* **Voltage Divider:** 10k resistor
* **OLED Display:** 0.92"
* **Power Supply:** 9V battery regulated to 3.3V
* **Additional Components:**
    * Capacitor on the analog pin (for stable ADC readings)
    * ON/OFF switch

### Circuit Diagram

This setup ensures a stable and accurate temperature reading. The 3.3V regulation is crucial for the 10k resistor in the voltage divider to provide a stable voltage, preventing fluctuations that could affect the final temperature value.

## Images

Here are images of the project:

<img src="https://github.com/DRIFTYY777/Thermistor-Atmega8A/blob/main/IMG_9539.jpeg" width="300">
<img src="https://github.com/DRIFTYY777/Thermistor-Atmega8A/blob/main/IMG_9540.jpeg" width="300">
<img src="https://github.com/DRIFTYY777/Thermistor-Atmega8A/blob/main/IMG_9541.jpeg" width="300">

## Key Considerations

* **Stable ADC Values:** Best practices were followed to ensure stable and accurate ADC readings. This includes using a capacitor on the analog pin and verifying the internal voltage reference.
* **Voltage Regulation:** The device operates on 3.3V, regulated from a 9V battery to ensure stable voltage for the 10k resistor and accurate temperature readings.

## Code

```c
// Code snippets or links to the code repository would go here
// Example:
// #include <avr/io.h>
// #include <util/delay.h>
// ...
