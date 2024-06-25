#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>

RTC_DS3231 rtc;
const int alarminute1 = 10; 
const int alarminute2 = 15;
volatile int startedSeconds;

void setAlarm1(DateTime now) {
  if(abs(now.second() - startedSeconds) % alarminute1 == 0){
    Serial.println("Alarm 1");
    digitalWrite(13, HIGH);
  }
}

void setAlarm2(DateTime now) {
  if(abs(now.second() - startedSeconds) % alarminute2 == 0){
    Serial.println("Alarm 2");
    digitalWrite(13, HIGH);
  }
}

void setup() {
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Hello World");
  
  // Initialize the LED pin as an output
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Initialize the RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); 
  }

  // Check if the RTC lost power
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  startedSeconds = rtc.now().second();
}

void loop() {
  DateTime now = rtc.now();

  // Print the current seconds
  Serial.println(now.second());

  // Check alarms
  setAlarm1(now);
  setAlarm2(now);

  // Wait for 1 second before checking again
  delay(1000);
}