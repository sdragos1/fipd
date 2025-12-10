#include <Arduino.h>

const int kButtonPin = 2;
const int kRedPin = 3;
const int kBluePin = 4;
const int kGreenPin = 5;

int buttonState = 0;
bool buttonPressed = false;
bool lastButtonState = false;
unsigned short clickCount = 0;

unsigned long prevMillis = 0;
const long interval = 500;

typedef struct {
  int red;
  int green;
  int blue;
  const char* name;
} Color;

int colorIndex = 0;

Color colors[] = {
  {LOW, HIGH, HIGH, "Red"},
  {HIGH, LOW, HIGH, "Green"},
  {HIGH, HIGH, LOW, "Blue"},
  {LOW, LOW, HIGH, "Yellow"},
  {LOW, HIGH, LOW, "Magenta"},
  {HIGH, LOW, LOW, "Cyan"},
  {LOW, LOW, LOW, "White"}
};


const int numColors = sizeof(colors) / sizeof(colors[0]);

void setup() {
  Serial.begin(9600);
  pinMode(kRedPin, OUTPUT);
  pinMode(kGreenPin, OUTPUT);
  pinMode(kBluePin, OUTPUT);
  pinMode(kButtonPin, INPUT);
}

void updateButtonState() {
  if (digitalRead(kButtonPin) == HIGH) {
    buttonPressed = true;
  } else {
    buttonPressed = false;
  }
}

void setColor(int index) {
  digitalWrite(kRedPin, colors[index].red);
  digitalWrite(kGreenPin, colors[index].green);
  digitalWrite(kBluePin, colors[index].blue);
}

unsigned long computeDeltaTime() {
  return millis() - prevMillis;
}

void loop() {
  updateButtonState();
  if (buttonPressed && !lastButtonState) {
    clickCount++;
    Serial.print("Click count: ");
    Serial.println(clickCount);
    if (clickCount >= 2) {
      clickCount = 0;
      colorIndex = (colorIndex + 1) % numColors;
      setColor(colorIndex);
      
      Serial.print("Color changed to: ");
      Serial.println(colors[colorIndex].name);
    }
  }

  if (computeDeltaTime() >= interval) {
    clickCount = 0;
    prevMillis = millis();
  }

  lastButtonState = buttonPressed;
}
