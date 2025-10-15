#include <Arduino.h>

const int digitalInPin = 2;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int sensorValue = 0;
bool prevButtonState = false;

int mood = 10;
const int moodMax = 20;

const float hslMax = 240.0;

unsigned long prevMillis = 0;
unsigned long accumulatedMillis = 0;
const long decreaseThreshold = 5000; // milliseconds

void setup()
{
    Serial.begin(9600);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}

void hslToRgb(int h, int s, int l, int *rgb)
{
    float hf = h / 60.0;
    float sf = s / 255.0;
    float lf = l / 255.0;

    int i = (int)hf;
    float f = hf - i;
    float p = lf * (1.0 - sf);
    float q = lf * (1.0 - f * sf);
    float t = lf * (1.0 - (1.0 - f) * sf);

    float r, g, b;

    switch (i % 6)
    {
    case 0:
        r = lf * (1.0 + sf);
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = lf;
        b = p;
        break;
    case 2:
        r = p;
        g = lf;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = lf;
        break;
    case 4:
        r = t;
        g = p;
        b = lf;
        break;
    case 5:
        r = lf;
        g = p;
        b = q;
        break;
    }

    rgb[0] = (int)(r * 255);
    rgb[1] = (int)(g * 255);
    rgb[2] = (int)(b * 255);
}

void updateLedColor()
{
    int rgb[3];

    int hue = map(mood, 0, moodMax, 0, hslMax);
    hslToRgb(hue, 255, 255, rgb);

    analogWrite(redPin, rgb[0]);
    analogWrite(greenPin, rgb[1]);
    analogWrite(bluePin, rgb[2]);
}

long computeDeltaTime()
{
    int currentMillis = millis();
    long deltaTime = currentMillis - prevMillis;
    prevMillis = currentMillis;
    return deltaTime;
}

void loop()
{
    sensorValue = digitalRead(digitalInPin);
    long deltaTime = computeDeltaTime();
    accumulatedMillis += deltaTime;
    if (accumulatedMillis >= decreaseThreshold + 1000)
    {
        mood -= 1;
        accumulatedMillis = decreaseThreshold;
        if (mood < 0)
        {
            mood = 0;
        }
        Serial.println(mood);
    }
    if (sensorValue == HIGH && !prevButtonState)
    {
        accumulatedMillis = 0;
        mood += 1;
        Serial.println(mood);
        if (mood > moodMax)
        {
            mood = moodMax;
        }
        prevButtonState = true;
    }
    if (sensorValue == LOW && prevButtonState)
    {
        prevButtonState = false;
    }

    updateLedColor();
    delay(20);
}
