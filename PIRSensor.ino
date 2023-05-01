// Arduino default library
#include <Wire.h>
#include <SPI.h>
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


#include <WiFi.h>
const char* ssid = "Tan Wifi@unifi";
const char* password = "59301357";

#include <WiFiClient.h>
WiFiClient client;

// Download and install thinkspeak
// https://github.com/mathworks/thingspeak-arduino
#include "ThingSpeak.h"
unsigned long myChannelNumber = 2119655;
const char * myWriteAPIKey = "BAKK01E3G8207XN4";

const char* host = "api.thingspeak.com";
const int httpsPort = 443;
const String apiKey = "BAKK01E3G8207XN4";


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
  tone(BUZZER_PIN, 5000);  // High frequency
  delay(300);
  tone(BUZZER_PIN, 8000);  // Low frequency
  delay(300);
  tone(BUZZER_PIN, 5000);  // High frequency
  delay(300);
  tone(BUZZER_PIN, 8000);  // Low frequency
  delay(300);
  noTone();
}

void refreshScreen() {
  lcd.clear();
  // DateTime now = rtc.now();
  sprintf(d, "%02d-%02d-%02d", lastTriggerDate.year(), lastTriggerDate.month(),lastTriggerDate.day());
  sprintf(t, "%02d:%02d:%02d", lastTriggerDate.hour(), lastTriggerDate.minute(), lastTriggerDate.second());
  Serial.print(F("Date/Time: "));
  Serial.println(t);

  lcd.setCursor(0, 0);
  lcd.print("Date:"+String(d));
  lcd.setCursor(0, 1);
  lcd.print("Time:"+String(t));
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

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    ThingSpeak.begin(client);

      // Use WiFiClientSecure for HTTPS
    WiFiClient client;

    // Connect to the server
    if (!client.connect(host, httpsPort)) {
      Serial.println("Connection failed");
      return;
    }

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
    lastTriggerDate = rtc.now();
    Serial.print("Motion Detected! ");
    refreshScreen();
    triggerAlarm();
    // Serial.println(String(d)+"T"+String(t)+"Z");
  
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    // int y = ThingSpeak.setCreatedAt(String(d)+" "+String(t));
    // if(y == 200){
    //   Serial.println("setCreatedAt update successful.");
    // }
    // else{
    //   Serial.println("Problem updating setCreatedAt. HTTP error code " + String(y));
    // }
    int x = ThingSpeak.writeField(myChannelNumber, 1, 1 ,  myWriteAPIKey);
    // String createdDate = String(d)+"T"+String(t)+"Z";
    // int x = ThingSpeak.writeRaw(6,"https://api.thingspeak.com/update.json?api_key=BAKK01E3G8207XN4&field5=1&created_at="+createdDate)
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    
    // Wait for the response
    delay(1000);
    while (client.available()) {
      Serial.write(client.read());
    }

      // Update ThingSpeak
  // Print Update Response to Serial Monitor

      delay(500);
  } else {
    pirSensorValue = 0;
  }
}


// ========================== Dont Use ===========================================
// Download DS3231 Library https://github.com/rodan/ds3231
// Example for DS3231 https://drive.google.com/file/d/0B6k7-e-fDhSZZHpFdnhSbkdUSkU/view?resourcekey=0-RCOExE9Yk6Ac95vaqHy4uw
// ========================== Dont Use End ===========================================