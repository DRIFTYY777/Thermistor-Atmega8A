// Simple I2C test for ebay 128x32 oled.
#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;

// Thermistor parameters and variables with pin number
const float vRef = 3.3; // Reference voltage for ADC (adjust as needed)
const int thermistorPin = A2;
const float seriesResistor = 10000.0;    // 10k resistor
const float nominalResistance = 10000.0; // at 25°C
const float nominalTemperature = 25.0;   // in Celsius
const float betaCoefficient = 3950.0;    // Beta value
const float adcMax = 1023.0;             // 10-bit ADC

//------------------------------------------------------------------------------
void setup()
{
  Wire.begin();
  Wire.setClock(400000L);

  // rotate screen
  oled.begin(&Adafruit128x32, I2C_ADDRESS, -1);
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.setCursor(0, 0);
  oled.set2X();
  oled.println("Adc Init");
  delay(200);
  oled.clear();
  oled.println("Temp init");
  delay(200);
  oled.clear();
}
//------------------------------------------------------------------------------
void loop()
{
  int adcValue = analogRead(thermistorPin);

  // Convert ADC value to voltage and then to resistance
  float voltage = adcValue * (vRef / adcMax);
  float resistance = (seriesResistor * voltage) / (vRef - voltage);

  // Convert resistance to temperature using Beta formula
  float steinhart;
  steinhart = resistance / nominalResistance;       // (R/Ro)
  steinhart = log(steinhart);                       // ln(R/Ro)
  steinhart /= betaCoefficient;                     // 1/B * ln(R/Ro)
  steinhart += 1.0 / (nominalTemperature + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                      // Invert
  steinhart -= 273.15;                              // Convert to Celsius

  float tempCalibrated = 1.028 * steinhart + 5.93; // Calibration factor

  // Print temperature to OLED
  oled.setCursor(0, 1);
  oled.set2X();
  oled.print("T:");
  // oled.print(steinhart, 2); // Print temperature with 2 decimal places
  oled.print(tempCalibrated, 2); // Print temperature with 2 decimal places
  oled.print("C");



  delay(10); // Update every second
}
