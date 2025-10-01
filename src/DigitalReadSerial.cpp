#include <Arduino.h>

const int pushButton = 2;
const int ledPin = 3;

int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(pushButton, INPUT);
}

void loop() {
  buttonState = digitalRead(pushButton);
  
  if (buttonState == HIGH) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }
}
