#include <Arduino.h>

const int pushButton = 2;
const int kRedPin = 3;
const int kBluePin = 4;
const int kGreenPin = 5;

int buttonState = 0;
bool ButtonIsPressed = false;
bool lastButtonState = false;
int colorIndex = 0;

typedef struct {
  int red;
  int green;
  int blue;
  const char* name;
} Color;

Color colors[] = {
  {HIGH, LOW, LOW, "Red"},
  {LOW, HIGH, LOW, "Green"},
  {LOW, LOW, HIGH, "Blue"},
  {HIGH, HIGH, LOW, "Yellow"},
  {HIGH, LOW, HIGH, "Magenta"},
  {LOW, HIGH, HIGH, "Cyan"},
  {HIGH, HIGH, HIGH, "White"},
  {LOW, LOW, LOW, "Off"}
};

const int numColors = 8;

void setup() {
  Serial.begin(9600);
  pinMode(kRedPin, OUTPUT);
  pinMode(kGreenPin, OUTPUT);
  pinMode(kBluePin, OUTPUT);
  pinMode(pushButton, INPUT);
}

void updateButtonState() {
  if (digitalRead(pushButton) == HIGH) {
    ButtonIsPressed = true;
  } else {
    ButtonIsPressed = false;
  }
}

void setColor(int index) {
  digitalWrite(kRedPin, colors[index].red);
  digitalWrite(kGreenPin, colors[index].green);
  digitalWrite(kBluePin, colors[index].blue);
}

void loop() {
  updateButtonState();

  if (ButtonIsPressed && !lastButtonState) {
    colorIndex = (colorIndex + 1) % numColors;
    setColor(colorIndex);
    
    Serial.print("Color changed to: ");
    Serial.println(colors[colorIndex].name);
  }

  lastButtonState = ButtonIsPressed;
  
  delay(50);
}