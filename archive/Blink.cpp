#include <Arduino.h>

const int kRedPin = 9;
const int kBluePin = 10;
const int kGreenPin = 11;

void setColor(unsigned int r, unsigned int g, unsigned int b)
{
  analogWrite(kRedPin, 255 - r);
  analogWrite(kGreenPin, 255 - g);
  analogWrite(kBluePin, 255 - b);
}

void setup() {
  pinMode(kRedPin, OUTPUT);
  pinMode(kBluePin, OUTPUT);
  pinMode(kGreenPin, OUTPUT);
}

int count = 255;
bool increasing = false;

void loop() {
  setColor(255 - count, 0, 0);
  delay(1);


  if (increasing) {
    count+=1;
    if (count >= 255) {
      increasing = false;
    }
  } else {
    count-=1;
    if (count <= 0) {
      increasing = true;
    }
  }
}
