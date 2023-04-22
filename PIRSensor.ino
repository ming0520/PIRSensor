// Download HT16K33 from Sketch > Include Library > Manage Libraries > Search HT16k33
// Download DS3231 Library https://github.com/rodan/ds3231
// Example for DS3231 https://drive.google.com/file/d/0B6k7-e-fDhSZZHpFdnhSbkdUSkU/view?resourcekey=0-RCOExE9Yk6Ac95vaqHy4uw
#include <HT16K33.h>

// Define 7 segment display
HT16K33  seg(0x70);
uint32_t start, stop;
uint8_t  ar[4];

// Define the PIR sensor input pin
int pirSensorPin = A0;

// Define Buzzer output pin
int buzzerPin = 8;

void triggerAlarm(){
  // Buzzer
  tone(buzzerPin, 1000); // Turn on the buzzer with a frequency of 1000Hz
  delay(1000); // Wait for 1 second
  noTone(buzzerPin); // Turn off the buzzer
}

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

  // Set the buzzer pin as output
  pinMode(buzzerPin, OUTPUT); 
}

void loop() {
  // Read the analog value from the PIR sensor
  int pirSensorValue = analogRead(pirSensorPin);
  Serial.print("Sensor value: ");
  Serial.print(pirSensorPin);
  Serial.println();

  // Convert the analog value to a digital value
  if (pirSensorValue > 512) {
    pirSensorValue = 1;
    Serial.println("Motion Detected!");
    // triggerAlarm();
  } else { 
    pirSensorValue = 0;
  }

  time_t t = now();
  // Serial.println(F(__DATE__));
  Serial.print(hour(t));
  Serial.print(":");
  Serial.println(minute(t));

  // seg.displayTime(hours, minutes);
  seg.displayColon(1);

  // Wait for a short time to prevent rapid triggering of the sensor
  delay(500);
}
