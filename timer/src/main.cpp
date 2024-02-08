#include <Arduino.h>
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
void showPinTime();
int state;
int led = D3;
int ledPin = D7;
int swSP = D4;
int swR = D5;
int swP = D6;
bool lastSwPState = HIGH;
bool lastSwSPState = HIGH;
bool lastSwRState = HIGH;
bool pinTimeRequested = false;
unsigned long start = 0;
unsigned long count = 0;
unsigned long sec = 0;
unsigned long minu = 0;
unsigned long milli = 0;
unsigned long previousMillis = 0;
const long interval = 500;
bool ledState = LOW;
String show, pinTimeText;

void setup()
{
  Serial.begin(115200);
  lcd.init();
  Wire.begin();
  pinMode(led, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(swSP, INPUT_PULLUP);
  pinMode(swR, INPUT_PULLUP);
  pinMode(swP, INPUT_PULLUP);
  lcd.backlight();
  state = 0;
}

void loop()
{
  bool currentSwPState = digitalRead(swP);
  bool currentSwSPState = digitalRead(swSP);
  bool currentSwRState = digitalRead(swR);
  if (state == 0)
  {
    lcd.setCursor(2, 0);
    lcd.print("Press button");
    lcd.setCursor(4, 1);
    lcd.print("to start");
    if (lastSwSPState == HIGH && currentSwSPState == LOW)
    {
      lcd.clear();
      state = 1;
      start = millis();
    }
    lastSwSPState = currentSwSPState;
  }
  if (state == 1)
  {
    count = millis() - start;
    minu = count / 60000;
    sec = (count / 1000) % 60;
    milli = count % 100;
    show = "Play  : " + String(minu) + ":" + String(sec) + ":" + String(milli) + " ";
    lcd.setCursor(0, 0);
    lcd.print(show);
    digitalWrite(led, HIGH);
    digitalWrite(ledPin, LOW);
    pinTimeRequested = false;
    if (lastSwPState == HIGH && currentSwPState == LOW)
    {
      pinTimeRequested = true;
      showPinTime();
    }
    lastSwPState = currentSwPState;
    if (lastSwSPState == HIGH && currentSwSPState == LOW)
    {
      state = 2;
    }
    lastSwSPState = currentSwSPState;
     if (lastSwRState == HIGH && currentSwRState == LOW)
    {
      lcd.clear();
      Serial.println("sr");
      digitalWrite(led, LOW);
      state = 0;
    }
    lastSwRState = currentSwRState;
  }
  if (state == 2)
  {
    unsigned long currentMillis = millis();
    start = millis() - count;
    show = "Pause : " + String(minu) + ":" + String(sec) + ":" + String(milli) + " ";
    lcd.setCursor(0, 0);
    lcd.print(show);
    pinTimeRequested = false;
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(led, ledState);
    }
    digitalWrite(ledPin, LOW);
    if (lastSwPState == HIGH && currentSwPState == LOW)
    {
      pinTimeRequested = true;
      showPinTime();
    }
    lastSwPState = currentSwPState;
    if (lastSwSPState == HIGH && currentSwSPState == LOW)
    {
      state = 1;
    }
    lastSwSPState = currentSwSPState;
    if (lastSwRState == HIGH && currentSwRState == LOW)
    {
      lcd.clear();
      Serial.println("sr");
      digitalWrite(led, LOW);
      state = 0;
    }
    lastSwRState = currentSwRState;
  }
}

void showPinTime()
{
  digitalWrite(ledPin, HIGH);
  pinTimeText = "Pin   : " + String(minu) + ":" + String(sec) + ":" + String(milli) + " ";
  lcd.setCursor(0, 1);
  lcd.print(pinTimeText);
  pinTimeRequested = false;
}
