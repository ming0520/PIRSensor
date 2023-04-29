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

// Download LiquidCrystal I2C by Frank de
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the PIR sensor input pin
int pirSensorPin = A7;

// Define Buzzer output pin
int BUZZER_PIN = 26;

DateTime lastTriggerDate;

int playing = 0;
void tone(byte pin, int freq) {
  ledcSetup(0, 2000, 8);   // setup beeper
  ledcAttachPin(pin, 0);   // attach beeper
  ledcWriteTone(0, freq);  // play tone
  playing = pin;           // store pin
}
void noTone() {
  tone(playing, 0);
}


void triggerAlarm() {
  tone(BUZZER_PIN, 1000);  // High frequency
  delay(100);
  tone(BUZZER_PIN, 2000);  // Low frequency
  delay(100);
  tone(BUZZER_PIN, 1000);  // High frequency
  delay(100);
  tone(BUZZER_PIN, 2000);  // Low frequency
  delay(100);
  noTone();
}

void refreshScreen() {
  lcd.clear();
  // DateTime now = rtc.now();
  sprintf(d, "Date:%02d/%02d/%02d", lastTriggerDate.day(), lastTriggerDate.month(), lastTriggerDate.year());
  sprintf(t, "%02d:%02d:%02d", lastTriggerDate.hour(), lastTriggerDate.minute(), lastTriggerDate.second());
  Serial.print(F("Date/Time: "));
  Serial.println(t);

  lcd.setCursor(0, 0);
  lcd.print(d);
  lcd.setCursor(0, 1);
  lcd.print(t);
  // Wait for a short time to prevent rapid triggering of the sensor
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  lcd.backlight();
  // Set the PIR sensor pin as an input
  pinMode(pirSensorPin, INPUT);

  // for DS3231
  rtc.begin();
  lcd.init();
  lastTriggerDate = rtc.now();
}

void loop() {
  // Read the analog value from the PIR sensor
  int pirSensorValue = analogRead(pirSensorPin);
  // Serial.print("Sensor value: ");
  // Serial.print(pirSensorPin);
  // Serial.println();

  // Convert the analog value to a digital value
  if (pirSensorValue > 512) {
    pirSensorValue = 1;
    Serial.println("Motion Detected!");
    lastTriggerDate = rtc.now();
    refreshScreen();
    triggerAlarm();
  } else {
    pirSensorValue = 0;
  }
}

// ========================== Dont Use ===========================================
// Download DS3231 Library https://github.com/rodan/ds3231
// Example for DS3231 https://drive.google.com/file/d/0B6k7-e-fDhSZZHpFdnhSbkdUSkU/view?resourcekey=0-RCOExE9Yk6Ac95vaqHy4uw
// ========================== Dont Use End ===========================================