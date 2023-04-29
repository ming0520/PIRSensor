// Arduino default library
#include <Wire.h>

// // Download HT16K33 from Sketch > Include Library > Manage Libraries > Search HT16k33
// #include <HT16K33.h>

// Download & Install RTClib by Adafruit
// https://circuitdigest.com/microcontroller-projects/interfacing-ds3231-rtc-with-arduino-and-diy-digital-clock
#include <RTClib.h>
RTC_DS3231 rtc;
char d[32];
char t[32];

// Install Adafruit LiquidCrystal by Adafruit from Manage Libraries
// #include "Adafruit_LiquidCrystal.h"
// Adafruit_LiquidCrystal lcd(0);


// Download LiquidCrystal I2C by Frank de
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 

// // Define 7 segment display
// HT16K33  seg(0x70);
// uint32_t start, stop;
// uint8_t  ar[4];

// Define the PIR sensor input pin
int pirSensorPin = A7;

// Define Buzzer output pin
int buzzerPin = 8;


// Arduino Code
// void triggerAlarm(){
//   // Buzzer
//   tone(buzzerPin, 1000); // Turn on the buzzer with a frequency of 1000Hz
//   delay(1000); // Wait for 1 second
//   noTone(buzzerPin); // Turn off the buzzer
// }
// End Arduino Code

DateTime lastTriggerDate;
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  lcd.backlight();
  // Set the PIR sensor pin as an input
  pinMode(pirSensorPin, INPUT);

  // LED Segment Initialize
  // seg.begin();
  // Wire.setClock(100000);

  // seg.displayOn();

  // seg.setDigits(4);
  // Serial.println("displayTest()");
  // seg.displayTest(1);
  // seg.displayOff();
  // delay(1000);
  // seg.displayOn();
  // seg.displayColon(false);
  // END LED Segment Initialize

  // Arduino Code
  // Set the buzzer pin as output
  // pinMode(buzzerPin, OUTPUT); 
  // End Arduino Code

// for DS3231
  rtc.begin();
  lcd.init();
  lastTriggerDate = rtc.now();

}


void loop() {
  // Read the analog value from the PIR sensor
  lcd.clear();
  int pirSensorValue = analogRead(pirSensorPin);
  Serial.print("Sensor value: ");
  Serial.print(pirSensorPin);
  Serial.println();

  // DateTime now = rtc.now();
  sprintf(d,"%02d/%02d/%02d",lastTriggerDate.day(), lastTriggerDate.month(), lastTriggerDate.year());
  sprintf(t, "%02d:%02d:%02d", lastTriggerDate.hour(), lastTriggerDate.minute(), lastTriggerDate.second());  
  Serial.print(F("Date/Time: "));
  Serial.println(t);

  // Convert the analog value to a digital value
  if (pirSensorValue > 512) {
    pirSensorValue = 1;
    Serial.println("Motion Detected!");
    lastTriggerDate = rtc.now();
    triggerAlarm();
  } else { 
    pirSensorValue = 0;
  }
  lcd.setCursor(0, 0);
  lcd.print(d);
  lcd.setCursor(0, 1);
  lcd.print(t);
  // Wait for a short time to prevent rapid triggering of the sensor
  delay(500);
  lcd.clear();

}



// ========================== Dont Use ===========================================
// Download DS3231 Library https://github.com/rodan/ds3231
// Example for DS3231 https://drive.google.com/file/d/0B6k7-e-fDhSZZHpFdnhSbkdUSkU/view?resourcekey=0-RCOExE9Yk6Ac95vaqHy4uw
// ========================== Dont Use End ===========================================