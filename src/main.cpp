#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>

SoftwareSerial mySerial(2, 3); 
Servo myservo;
RTC_DS3231 rtc;

String incommingData = "";
volatile int alarminute1 = 2; 
volatile int alarminute2 = 3;
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
    Serial.println("Alarm 1");
    turnServo();
    delay(1000);
  }
}



void setAlarm2(long passedTime) {
  if(passedTime % (alarminute2*60) == 0){
    Serial.println("Alarm 2");
  }
}


void ConnectionBluetooth(){
  if(mySerial.available()>0){
    incommingData = mySerial.readString();
    int data = incommingData.toInt();
    if(data <= 27){
      analogWrite(11,255);
      analogWrite(10,255);
      analogWrite(6,255);
    }else if(data > 27 && data < 35){
      long tmp = map(data, 38, 0, 0, 255);
      analogWrite(11,tmp);
      analogWrite(10,tmp);
      analogWrite(6,0);
      Serial.println("used tmp :"+String(tmp));
    }else{
      long tmp = map(data, 38, 0, 0, 255);
      analogWrite(11,tmp);
      analogWrite(10,0);
      Serial.println("used tmp :"+String(tmp));
      analogWrite(6,0);
    }

  }
}

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(13, OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(13, LOW);
  analogWrite(6,255);
  analogWrite(11,255);
  analogWrite(10,255);

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
  ConnectionBluetooth();
  DateTime now = rtc.now();
  long elapse = (now - startedMinutes).totalseconds();
  setAlarm1(elapse);
  setAlarm2(elapse);
}
