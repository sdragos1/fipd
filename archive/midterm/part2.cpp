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
    OFF,
    POLICE_SIREN,
};

Mode currentMode = RAINBOW_CYCLE;
Mode modeBeforeOff = RAINBOW_CYCLE; // Remember mode before turning off

unsigned short kTotalModes = 3; // Only cycle through first 3 modes

unsigned long prevMillis = 0;

unsigned long modeSwitchInterval = 500;
unsigned long modeSwitchAccumulator = 0;

unsigned long colorSwitchInterval = 1000;
unsigned long colorSwitchAccumulator = 0;

unsigned long blinkInterval = 500;
unsigned long blinkAccumulator = 0;

unsigned long fadeAccumulator = 0;
unsigned long sirenAccumulator = 0;

// Button hold detection variables
unsigned long buttonPressStartTime = 0;
bool buttonWasPressed = false;
bool mediumPressTriggered = false;
bool longPressTriggered = false;
const unsigned long mediumPressDuration = 500; // 0.5 seconds to turn off
const unsigned long longPressDuration = 1500;  // 2 seconds for police siren

int sensorValue = 0;

void refreshColor()
{
    analogWrite(kRedPin, 255 - currColor.r);
    analogWrite(kGreenPin, 255 - currColor.g);
    analogWrite(kBluePin, 255 - currColor.b);
}

void setColor(int r, int g, int b)
{
    analogWrite(kRedPin, 255 - r);
    analogWrite(kGreenPin, 255 - g);
    analogWrite(kBluePin, 255 - b);
}

void turnOff()
{
    analogWrite(kRedPin, 255);
    analogWrite(kGreenPin, 255);
    analogWrite(kBluePin, 255);
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
    sirenAccumulator += delta;
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

void policeSiren()
{
    unsigned long sirenInterval = 15;   // Smooth transition update rate
    static int brightness = 0;          // Current brightness (0-255)
    static int direction = 1;           // 1=getting brighter, -1=getting dimmer
    static bool isRed = true;           // true=red phase, false=blue phase
    static int stepSize = 3;            // How fast to fade
    
    if (sirenAccumulator >= sirenInterval)
    {
        sirenAccumulator = 0;
        
        brightness += direction * stepSize;
        
        // Check if we've completed a fade cycle
        if (brightness >= 255)
        {
            brightness = 255;
            direction = -1;  // Start fading out
        }
        else if (brightness <= 0)
        {
            brightness = 0;
            direction = 1;   // Start fading in
            isRed = !isRed;  // Switch colors when fully dimmed
        }
        
        // Set the color based on current phase
        if (isRed)
        {
            setColor(brightness, 0, 0);  // Red breathing
        }
        else
        {
            setColor(0, 0, brightness);  // Blue breathing
        }
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
        mediumPressTriggered = false;
        longPressTriggered = false;
    }

    if (sensorValue == LOW && buttonWasPressed)
    {
        unsigned long pressDuration = millis() - buttonPressStartTime;

        if (pressDuration >= longPressDuration && !longPressTriggered)
        {
            longPressTriggered = true;
            mediumPressTriggered = true;
            currentMode = POLICE_SIREN;
            sirenAccumulator = 0;
        }
        else if (pressDuration >= mediumPressDuration && !mediumPressTriggered)
        {
            mediumPressTriggered = true;

            if (currentMode != OFF && currentMode != POLICE_SIREN)
            {
                modeBeforeOff = currentMode;
            }

            currentMode = OFF;
            turnOff();
        }
    }

    if (sensorValue == HIGH && !buttonWasPressed)
    {
        currentMode = modeBeforeOff;
    }

    if (sensorValue == HIGH && buttonWasPressed)
    {
        buttonWasPressed = false;

        if (!mediumPressTriggered && !longPressTriggered)
        {

            currentMode = (Mode)((currentMode + 1) % kTotalModes);
            modeBeforeOff = currentMode;
            refreshColor();
        }
    }
    if (currentMode != RAINBOW_CYCLE) {
        currRainbowIndex = -1;
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

    case OFF:
        turnOff();
        break;

    case POLICE_SIREN:
        turnOff();
        break;

    default:
        break;
    }

    delay(10);
}