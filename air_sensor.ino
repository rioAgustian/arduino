#include <SoftwareSerial.h>
#include "Adafruit_PM25AQI.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
// Note: If your LCD stays blank, your I2C address might be 0x3F instead of 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Set up SoftwareSerial for the PMS7003 sensor (RX on Pin 2, TX on Pin 3)
SoftwareSerial pmSerial(2, 3);
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

void setup() {
  Serial.begin(115200);
  
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  
  // Display a startup message
  lcd.setCursor(0, 0);
  lcd.print("Air Quality");
  lcd.setCursor(0, 1);
  lcd.print("Monitor Booting.");
  
  delay(1000); // Wait for sensor to boot

  pmSerial.begin(9600);

  // Check if sensor is connected
  if (!aqi.begin_UART(&pmSerial)) {
    Serial.println("Could not find PM 2.5 sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring");
    while (1) { delay(10); } // Halt program
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Found!");
  delay(1000);
  lcd.clear();
}

void loop() {
  PM25_AQI_Data data;

  // Try to read data
  if (!aqi.read(&data)) {
    Serial.println("Could not read from sensor. Retrying...");
    delay(500);
    return;
  }
  
  // Print to Serial Monitor for debugging
  Serial.print("PM1.0: "); Serial.print(data.pm10_standard);
  Serial.print("\tPM2.5: "); Serial.print(data.pm25_standard);
  Serial.print("\tPM10: "); Serial.println(data.pm100_standard);

  // Format and print to the LCD
  // Row 0: "PM2.5: 123 ug/m3" (Max 16 characters)
  lcd.setCursor(0, 0);
  lcd.print("PM2.5: ");
  lcd.print(data.pm25_standard);
  lcd.print(" ug/m3   "); // Extra spaces clear leftover numbers from previous readings

  // Row 1: "PM1:123 PM10:123" (Max 16 characters)
  lcd.setCursor(0, 1);
  lcd.print("PM1:");
  lcd.print(data.pm10_standard);
  lcd.print("  PM10:");
  lcd.print(data.pm100_standard);
  lcd.print("    "); // Extra spaces to overwrite old characters

  // Wait 2 seconds before the next update
  delay(2000);
}