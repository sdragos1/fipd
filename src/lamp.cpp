#include <Arduino.h>

const int RED_PIN = 9;
const int BLUE_PIN = 10;
const int GREEN_PIN = 11;
const int BUTTON_PIN = 2;

enum LampMode
{
    OFF,
    NIGHT_FURY_BLACK,
    DRAGON_FIRE,      
    VIKING_BLUE,
    RAINBOW_FLIGHT,
    BREATHING_DRAGON,
    PLASMA_BLAST,
    SUNSET_BERK,
    AURORA_SKY
};

LampMode currentMode = OFF;
const int TOTAL_MODES = 9;

unsigned long lastButtonPress = 0;
const unsigned long DEBOUNCE_DELAY = 250;
int lastButtonState = HIGH;

unsigned long lastUpdate = 0;
float breathPhase = 0;
float rainbowPhase = 0;
unsigned long flickerTime = 0;

void setup()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 255);
}

void setColor(int r, int g, int b)
{
    analogWrite(RED_PIN, 255 - r);
    analogWrite(GREEN_PIN, 255 - g);
    analogWrite(BLUE_PIN, 255 - b);
}

void nightFuryEffect()
{
    setColor(20, 0, 80);
}

void dragonFireEffect()
{
    int flicker = random(180, 255);
    int r = flicker;
    int g = random(40, flicker / 3);
    int b = 0;
    setColor(r, g, b);
    delay(random(50, 150));
}

void vikingBlueEffect()
{
    setColor(0, 100, 255);
}

void rainbowFlightEffect()
{
    rainbowPhase += 0.01;
    if (rainbowPhase > 6.28)
        rainbowPhase = 0;

    int r = (sin(rainbowPhase) * 127 + 128);
    int g = (sin(rainbowPhase + 2.09) * 127 + 128);
    int b = (sin(rainbowPhase + 4.18) * 127 + 128);

    setColor(r, g, b);
}

void breathingDragonEffect()
{
    breathPhase += 0.02;
    if (breathPhase > 6.28)
        breathPhase = 0;

    float brightness = (sin(breathPhase) + 1) / 2;
    int val = brightness * 255;

    setColor(val / 3, 0, val);
}

void plasmaBlastEffect()
{
    breathPhase += 0.05;
    if (breathPhase > 6.28)
        breathPhase = 0;

    float brightness = (sin(breathPhase) + 1) / 2;
    int r = brightness * 150;
    int b = brightness * 255;

    setColor(r, 0, b);
}

void sunsetBerkEffect()
{
    breathPhase += 0.005;
    if (breathPhase > 6.28)
        breathPhase = 0;

    float shift = (sin(breathPhase) + 1) / 2;
    int r = 255;
    int g = 80 + (shift * 100);
    int b = shift * 50;

    setColor(r, g, b);
}

void auroraSkyEffect()
{
    rainbowPhase += 0.015;
    if (rainbowPhase > 6.28)
        rainbowPhase = 0;

    float wave1 = (sin(rainbowPhase) + 1) / 2;
    float wave2 = (sin(rainbowPhase * 1.3 + 1) + 1) / 2;

    int r = wave1 * 100;
    int g = wave2 * 255;
    int b = wave1 * 200 + wave2 * 55;

    setColor(r, g, b);
}

void loop()
{
    int buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == LOW && lastButtonState == HIGH)
    {
        if (millis() - lastButtonPress > DEBOUNCE_DELAY)
        {
            currentMode = (LampMode)((currentMode + 1) % TOTAL_MODES);
            lastButtonPress = millis();
            breathPhase = 0;
            rainbowPhase = 0;
        }
    }
    lastButtonState = buttonState;

    switch (currentMode)
    {
    case OFF:
        setColor(0, 0, 0);
        break;
    case NIGHT_FURY_BLACK:
        nightFuryEffect();
        break;
    case DRAGON_FIRE:
        dragonFireEffect();
        break;
    case VIKING_BLUE:
        vikingBlueEffect();
        break;
    case RAINBOW_FLIGHT:
        rainbowFlightEffect();
        break;
    case BREATHING_DRAGON:
        breathingDragonEffect();
        break;
    case PLASMA_BLAST:
        plasmaBlastEffect();
        break;
    case SUNSET_BERK:
        sunsetBerkEffect();
        break;
    case AURORA_SKY:
        auroraSkyEffect();
        break;
    }

    delay(20);
}