#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <Servo.h>


Servo myservo;
RTC_DS3231 rtc;


const int alarminute1 = 2; 
const int alarminute2 = 3;
DateTime startedMinutes;
volatile int degrees[] = {0,90,180};
volatile int index = 0;
volatile bool goBack = false;

void turnServo() {
  if (index == 2) {
    goBack = true;
  }
  if (index == 0) {
    goBack = false;
  }
  
  myservo.write(degrees[index]);
  index = goBack ? index - 1 : index + 1;
}

void setAlarm1(long passedTime) {
  if(passedTime % (alarminute1*60) == 0){
    digitalWrite(13, HIGH);
    Serial.println("Alarm 1");
    turnServo();
    delay(1000);
  }
}



void setAlarm2(long passedTime) {
  if(passedTime % (alarminute2*60) == 0){
    digitalWrite(13, LOW);
    Serial.println("Alarm 2");
  }
}

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); 
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  startedMinutes = rtc.now();
  index = -1;
  turnServo();
}

void loop() {
  DateTime now = rtc.now();
  long elapse = (now - startedMinutes).totalseconds();
  Serial.println(elapse);
  
  setAlarm1(elapse);
  setAlarm2(elapse);
  
}
