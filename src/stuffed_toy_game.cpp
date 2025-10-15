#include <Arduino.h>

// My own libraries
#include <hsl.hpp>
#include <rgb.hpp>
// ---------------------

const int digitalInPin = 2;
const int redPin = 9;
const int greenPin = 11;
const int bluePin = 10;

int sensorValue = 0;
bool prevButtonState = false;

int mood = 10;
const int moodMax = 20;

const float hueMax = 200.0;

unsigned long prevMillis = 0;
unsigned long accumulatedMillis = 0;
const long decreaseThreshold = 5000;

void setup()
{
    Serial.begin(9600);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}

void updateLedColor()
{
    float hue = (float)mood / (float)moodMax * hueMax;

    Serial.print("Hue: ");
    Serial.println(hue);

    ColorRGB rgbColor = ColorHSL::fromHue(hue).toRGB();

    Serial.print("R: ");
    Serial.print(rgbColor.r);
    Serial.print(" G: ");
    Serial.print(rgbColor.g);
    Serial.print(" B: ");
    Serial.println(rgbColor.b);

    analogWrite(redPin, 255 - rgbColor.r);
    analogWrite(greenPin, 255 - rgbColor.g);
    analogWrite(bluePin, 255 - rgbColor.b);
}

void updateTime()
{
    int currentMillis = millis();
    long deltaTime = currentMillis - prevMillis;

    prevMillis = currentMillis;
    accumulatedMillis += deltaTime;
}

void checkMoodDecrease()
{
    if (accumulatedMillis >= decreaseThreshold + 1000)
    {
        mood -= 1;
        Serial.println(accumulatedMillis);
        accumulatedMillis = decreaseThreshold;
        if (mood < 0)
        {
            mood = 0;
        }
        Serial.println("Decrease mood:");
        Serial.println(mood);
    }
}

void checkMoodIncrease()
{
    if (sensorValue == LOW && !prevButtonState)
    {
        accumulatedMillis = 0;
        mood += 1;
        Serial.println("Increase mood:");
        Serial.println(mood);
        if (mood > moodMax)
        {
            mood = moodMax;
        }
        prevButtonState = true;
    }
    if (sensorValue == HIGH && prevButtonState)
    {
        prevButtonState = false;
    }
}

void loop()
{
    sensorValue = digitalRead(digitalInPin);
 
    updateTime();
    checkMoodDecrease();
    checkMoodIncrease();
    updateLedColor();
 
    delay(1);
}
