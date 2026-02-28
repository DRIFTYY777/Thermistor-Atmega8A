// Simple I2C test for ebay 128x32 oled.
#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define I2C_ADDRESS 0x3C
#define ADC_SAMPLES 100

bool samplesFilled = false;
uint8_t adcIndex = 0;
uint16_t adcValues[ADC_SAMPLES];

SSD1306AsciiWire oled;

// Thermistor parameters and variables with pin number
const float vRef = 3.3; // Reference voltage for ADC (adjust as needed)
const int thermistorPin = A2;
const float seriesResistor = 10000.0;    // 10k resistor
const float nominalResistance = 10000.0; // at 25°C
const float nominalTemperature = 25.0;   // in Celsius
const float betaCoefficient = 3950.0;    // Beta value
const float adcMax = 1023.0;             // 10-bit ADC

// Interrupt Service Routine for ADC complete
ISR(ADC_vect)
{
  // Read the 10-bit result (ADCL must be read first)
  // Or use the ADCW macro which handles both 8-bit registers
  // adc_result = ADCW;
  uint16_t adcResult = ADC;                // Read ADC result
  adcValues[adcIndex] = adcResult;         // Store in circular buffer
  adcIndex = (adcIndex + 1) % ADC_SAMPLES; // Increment index with wrap-around

  if (adcIndex >= ADC_SAMPLES - 1)
  {
    adcIndex = 0;         // Reset index after filling the buffer
    samplesFilled = true; // Set flag to indicate buffer is filled
  }

  // Optional: Start the next conversion if not in Free Running mode
  // ADCSRA |= (1 << ADSC);
}

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
  if (samplesFilled)
  {
    // Calculate average ADC value
    uint32_t adcSum = 0;
    for (uint8_t i = 0; i < ADC_SAMPLES; i++)
    {
      adcSum += adcValues[i];
    }
    uint16_t adcValue = adcSum / ADC_SAMPLES; // Average ADC value

    // Reset flag for next round of sampling
    samplesFilled = false;

    // Process the averaged ADC value to calculate temperature
    float voltage = adcValue * (vRef / adcMax);
    float resistance = (seriesResistor * voltage) / (vRef - voltage);

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
    oled.print(tempCalibrated, 2); // Print temperature with 2 decimal places
    oled.print("C");
  }
}
