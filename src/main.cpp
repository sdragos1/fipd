#include <Arduino.h>

#include "rgb.hpp"

const int kButtonPin = 2;

const int kRedPin = 9;
const int kBluePin = 10;
const int kGreenPin = 11;

int buttonState = 0;

ColorRGB rainbowColors[] = {
    ColorRGB::kRed,
    ColorRGB::kYellow,
    ColorRGB::kGreen,
    ColorRGB::kBlue,
    ColorRGB::kMagenta,
};
int currRainbowIndex = 0;
ColorRGB currColor = ColorRGB::kRed;

enum Mode
{
    RAINBOW_CYCLE,
    WHITE_FADE,
};

enum RainbowType {
    RAINBOW_FULL,
    RAINBOW_BLINK,
};

Mode currentMode = RAINBOW_CYCLE;
RainbowType currentRainbowType = RAINBOW_FULL;

unsigned long prevMillis = 0;

unsigned long modeSwitchInterval = 500;
unsigned long modeSwitchAccumulator = 0;

unsigned long colorSwitchInterval = 1000;
unsigned long colorSwitchAccumulator = 0;

unsigned long blinkInterval = 250;
unsigned long blinkAccumulator = 0;

unsigned long fadeAccumulator = 0;

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
    if (currentRainbowType == RAINBOW_BLINK)
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
                nextColor();
                refreshColor();
            }
            isOn = !isOn;
        }
        return;
    }
    if (colorSwitchAccumulator >= colorSwitchInterval)
    {
        colorSwitchAccumulator = 0;
        
        nextColor();
        refreshColor();
    }
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

    if (sensorValue == LOW && !prevButtonState)
    {
        prevButtonState = true;
        currentMode = WHITE_FADE;
        fadeAccumulator = 0;
    }
    if (sensorValue == HIGH && prevButtonState)
    {
        prevButtonState = false;
        currentMode = RAINBOW_CYCLE;
        currentRainbowType = (RainbowType)((currentRainbowType + 1) % 2);
        refreshColor();
    }

    if (currentMode == RAINBOW_CYCLE)
    {
        rainbowCycle();
    }
    if (currentMode == WHITE_FADE)
    {
        whiteFade();
    }

    delay(10);
}