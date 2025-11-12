#include <Arduino.h>

#include "rgb.hpp"

const int kButtonPin = 2;

const int kRedPin = 9;
const int kBluePin = 10;
const int kGreenPin = 11;

int buttonState = 0;

ColorRGB rainbowColors[] = {
    ColorRGB::kGreen,
    ColorRGB::kYellow,
    ColorRGB::kRed,
};
int currRainbowIndex = 0;
ColorRGB currColor = ColorRGB::kGreen;

enum Mode
{
    RAINBOW_CYCLE,
    YELLOW_BLINK,
    RED_BLINK,
};

Mode currentMode = RAINBOW_CYCLE;

unsigned short kTotalModes = sizeof(rainbowColors) / sizeof(ColorRGB);

unsigned long prevMillis = 0;

unsigned long modeSwitchInterval = 500;
unsigned long modeSwitchAccumulator = 0;

unsigned long colorSwitchInterval = 2000;
unsigned long colorSwitchAccumulator = 0;

unsigned long blinkInterval = 250;
unsigned long blinkAccumulator = 0;

unsigned long fadeAccumulator = 0;

unsigned long buttonPressStartTime = 0;
bool buttonWasPressed = false;
bool longPressTriggered = false;
const unsigned long longPressDuration = 1500;
const unsigned long longPressDuration = 1500;

int sensorValue = 0;
bool prevButtonState = false;

void refreshColor()
{
    analogWrite(kRedPin, 255 - currColor.r);
    analogWrite(kGreenPin, 255 - currColor.g);
    analogWrite(kBluePin, 255 - currColor.b);
}

void setWhiteFade(int brightness)
{
    analogWrite(kRedPin, 255 - brightness);
    analogWrite(kGreenPin, 255 - brightness);
    analogWrite(kBluePin, 255 - brightness);
}

void setup()
{
    Serial.begin(9600);
    pinMode(kRedPin, OUTPUT);
    pinMode(kGreenPin, OUTPUT);
    pinMode(kBluePin, OUTPUT);
    pinMode(kButtonPin, INPUT);
    refreshColor();
}

void updateTime()
{
    unsigned long currentMillis = millis();
    unsigned long delta = currentMillis - prevMillis;
    prevMillis = currentMillis;
    colorSwitchAccumulator += delta;
    blinkAccumulator += delta;
    fadeAccumulator += delta;
}

void nextColor()
{
    currRainbowIndex = (currRainbowIndex + 1) % (sizeof(rainbowColors) / sizeof(ColorRGB));
    currColor = rainbowColors[currRainbowIndex];
    refreshColor();
}

void rainbowCycle()
{
    if (colorSwitchAccumulator >= colorSwitchInterval)
    {
        colorSwitchAccumulator = 0;

        nextColor();
        refreshColor();
    }
}

void blink()
{
    if (blinkAccumulator >= blinkInterval)
    {
        blinkAccumulator = 0;
        static bool isOn = true;
        if (isOn)
        {
            analogWrite(kRedPin, 255);
            analogWrite(kGreenPin, 255);
            analogWrite(kBluePin, 255);
        }
        else
        {
            refreshColor();
        }
        isOn = !isOn;
    }
    return;
}

void whiteFade()
{
    unsigned long fadeInterval = 30;
    static int fadeBrightness = 0;
    static int fadeDirection = 1;
    static int fadeStep = 5;

    if (fadeAccumulator >= fadeInterval)
    {
        fadeAccumulator = 0;

        fadeBrightness += fadeDirection * fadeStep;

        if (fadeBrightness >= 255)
        {
            fadeBrightness = 255;
            fadeDirection = -1;
        }
        else if (fadeBrightness <= 0)
        {
            fadeBrightness = 0;
            fadeDirection = 1;
        }

        setWhiteFade(fadeBrightness);
    }
}

void loop()
{
   int sensorValue = digitalRead(kButtonPin);

    updateTime();

    if (sensorValue == LOW && !buttonWasPressed)
    {
        buttonWasPressed = true;
        buttonPressStartTime = millis();
        longPressTriggered = false;
    }

    if (sensorValue == LOW && buttonWasPressed && !longPressTriggered)
    {
        if (millis() - buttonPressStartTime >= longPressDuration)
        {
            longPressTriggered = true;
            fadeAccumulator = 0;
        }
    }

    if (sensorValue == HIGH && buttonWasPressed)
    {
        buttonWasPressed = false;
        
        if (!longPressTriggered)
        {
            currentMode = (Mode)((currentMode + 1) % kTotalModes);
        } else {
            whiteFade();
        }
    }

    switch (currentMode)
    {
    case RAINBOW_CYCLE:
        rainbowCycle();
        break;

    case YELLOW_BLINK:
        currColor = ColorRGB::kYellow;
        blink();
        break;
        
    case RED_BLINK:
        currColor = ColorRGB::kRed;
        blink();
        break;
        
    default:
        break;
    }

    delay(10);
}
