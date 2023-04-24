// Arduino default library
#include <Wire.h>

// Download HT16K33 from Sketch > Include Library > Manage Libraries > Search HT16k33
#include <HT16K33.h>

// Download & Install RTClib by Adafruit
// https://circuitdigest.com/microcontroller-projects/interfacing-ds3231-rtc-with-arduino-and-diy-digital-clock
#include <RTClib.h>

// Define 7 segment display
HT16K33  seg(0x70);
uint32_t start, stop;
uint8_t  ar[4];

// Define the PIR sensor input pin
int pirSensorPin = A7;

// Define Buzzer output pin
int buzzerPin = 8;

RTC_DS3231 rtc;
char t[32];

// Arduino Code
// void triggerAlarm(){
//   // Buzzer
//   tone(buzzerPin, 1000); // Turn on the buzzer with a frequency of 1000Hz
//   delay(1000); // Wait for 1 second
//   noTone(buzzerPin); // Turn off the buzzer
// }
// End Arduino Code

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Set the PIR sensor pin as an input
  pinMode(pirSensorPin, INPUT);

  // LED Segment Initialize
  seg.begin();
  Wire.setClock(100000);

  seg.displayOn();

  seg.setDigits(4);
  Serial.println("displayTest()");
  seg.displayTest(1);
  seg.displayOff();
  delay(1000);
  seg.displayOn();
  seg.displayColon(false);

  // Arduino Code
  // Set the buzzer pin as output
  // pinMode(buzzerPin, OUTPUT); 
  // End Arduino Code

// for DS3231
  rtc.begin();
}

void loop() {
  // Read the analog value from the PIR sensor
  int pirSensorValue = analogRead(pirSensorPin);
  Serial.print("Sensor value: ");
  Serial.print(pirSensorPin);
  Serial.println();

  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  Serial.print(F("Date/Time: "));
  Serial.println(t);

  // Convert the analog value to a digital value
  if (pirSensorValue > 512) {
    pirSensorValue = 1;
    Serial.println("Motion Detected!");
    // triggerAlarm();
  } else { 
    pirSensorValue = 0;
  }

  

 // seg.displayTime(hours, minutes);
  seg.displayColon(1);

  // Wait for a short time to prevent rapid triggering of the sensor
  delay(500);
}



// ========================== Dont Use ===========================================
// Download DS3231 Library https://github.com/rodan/ds3231
// Example for DS3231 https://drive.google.com/file/d/0B6k7-e-fDhSZZHpFdnhSbkdUSkU/view?resourcekey=0-RCOExE9Yk6Ac95vaqHy4uw
// ========================== Dont Use End ===========================================