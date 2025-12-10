#include <Arduino.h>

const int analogInPin = A0;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int sensorValue = 0;
int hue = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}


void hsvToRgb(int h, int s, int v, int *rgb)
{
    float hf = h / 60.0;
    float sf = s / 255.0;
    float vf = v / 255.0;

    int i = (int)hf;
    float f = hf - i;
    float p = vf * (1.0 - sf);
    float q = vf * (1.0 - f * sf);
    float t = vf * (1.0 - (1.0 - f) * sf);

    float r, g, b;

    switch (i % 6)
    {
    case 0:
        r = vf;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = vf;
        b = p;
        break;
    case 2:
        r = p;
        g = vf;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = vf;
        break;
    case 4:
        r = t;
        g = p;
        b = vf;
        break;
    case 5:
        r = vf;
        g = p;
        b = q;
        break;
    }

    rgb[0] = (int)(r * 255);
    rgb[1] = (int)(g * 255);
    rgb[2] = (int)(b * 255);
}

void loop()
{
    sensorValue = digitalRead(digitalIn);

    hue = map(sensorValue, 0, 1023, 0, 366);

    int rgb[3];
    hsvToRgb(hue, 255, 255, rgb);

    analogWrite(redPin, rgb[0]);
    analogWrite(greenPin, rgb[1]);
    analogWrite(bluePin, rgb[2]);

    delay(20);
}
