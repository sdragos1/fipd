#include <Arduino.h>
#include <math.h>
#include <rgb.hpp>
#include <hsl.hpp>
#include <hsv.hpp>
#include "../lib/components/rgb_led_anode.hpp"

const uint8_t LED1_PIN = 5;
const uint8_t LED2_PIN = 3;

unsigned long prevMillis = 0;
unsigned long modeStartMillis = 0;
const unsigned long MODE_DURATION = 9000UL;
int modeIndex = 0;
const int MODE_COUNT = 6;

inline uint8_t fToByte(float v)
{
    v = constrain(v, 0.0f, 1.0f);
    return (uint8_t)roundf(v * 255.0f);
}

RGBLedAnode rgb;

static float easeInOutSine(float x)
{
    return 0.5f * (1 - cosf(x * M_PI));
}

static float phaseAt(unsigned long t, float periodSeconds)
{
    return fmodf((t / 1000.0f) / periodSeconds, 1.0f);
}

static float mixf(float a, float b, float t)
{
    return a + (b - a) * t;
}

static float shimmer(unsigned long t, float baseFreq, int octaves, float amp)
{
    float s = 0;
    for (int i = 0; i < octaves; ++i)
    {
        float f = baseFreq * (i + 1);
        s += amp / (i + 1) * sinf((t / 1000.0f) * f + i * 1.3f);
    }
    return constrain(s, -1.0f, 1.0f);
}

void effectAurora(unsigned long t)
{
    float s = t / 1000.0f;
    float baseHue = fmodf(150.0f + 40.0f * sinf(s * 0.08f) + 30.0f * sinf(s * 0.02f), 360.0f);
    float centerB = 0.55f + 0.25f * sinf(s * 0.4f) + 0.06f * shimmer(t, 6.0f, 3, 0.6f);
    centerB = constrain(centerB, 0.1f, 1.0f);
    float sat = 0.6f + 0.25f * sinf(s * 0.12f + 0.6f);

    ColorRGB center = ColorHSV(baseHue, sat * 100.0f, centerB * 100.0f).toRGB();
    rgb.SetColor(center);

    float leftTint = 0.25f + 0.15f * sinf(s * 0.9f);
    float rightTint = 0.25f + 0.15f * sinf(s * 0.9f + 1.1f);

    float leftBright = 0.15f + 0.45f * easeInOutSine(phaseAt(t, 3.5f)) * leftTint + 0.08f * shimmer(t, 14.0f, 2, 0.5f);
    float rightBright = 0.15f + 0.45f * easeInOutSine(phaseAt(t + 1000, 3.5f)) * rightTint + 0.08f * shimmer(t + 1200, 12.0f, 2, 0.5f);

    analogWrite(LED1_PIN, fToByte(constrain(leftBright, 0.0f, 1.0f)));
    analogWrite(LED2_PIN, fToByte(constrain(rightBright, 0.0f, 1.0f)));
}

void effectTriPulse(unsigned long t)
{
    float s = t / 1000.0f;
    float hue = fmodf((s * 30.0f) + 10.0f * sinf(s * 0.7f), 360.0f);
    float centerV = 0.6f + 0.35f * sinf(s * 0.9f);
    rgb.SetColor(ColorHSV(hue, 100.0f, centerV * 100.0f).toRGB());

    float leftPulse = 0.25f + 0.55f * 0.5f * (1 + sinf(s * 1.8f));
    float rightPulse = 0.25f + 0.55f * 0.5f * (1 + sinf(s * 1.8f + M_PI * 0.9f));
    analogWrite(LED1_PIN, fToByte(constrain(leftPulse, 0.0f, 1.0f)));
    analogWrite(LED2_PIN, fToByte(constrain(rightPulse, 0.0f, 1.0f)));
}

void effectColorSwipe(unsigned long t)
{
    float period = 2.2f;
    float p = phaseAt(t, period);
    float pos = p * 2.0f;

    float hue = fmodf(200.0f * p + 60.0f * sinf(t / 1000.0f * 0.2f), 360.0f);
    float leftB = 1.0f - min(1.0f, fabs(pos - 0.0f));
    float midB = 1.0f - min(1.0f, fabs(pos - 1.0f));
    float rightB = 1.0f - min(1.0f, fabs(pos - 2.0f));

    leftB = powf(leftB, 1.8f);
    midB = powf(midB, 1.6f);
    rightB = powf(rightB, 1.8f);

    rgb.SetColor(ColorHSV(hue, 90.0f, (0.6f + 0.4f * midB) * 100.0f).toRGB());
    analogWrite(LED1_PIN, fToByte(constrain(0.15f + 0.85f * leftB, 0.0f, 1.0f)));
    analogWrite(LED2_PIN, fToByte(constrain(0.15f + 0.85f * rightB, 0.0f, 1.0f)));
}

void effectMeteor(unsigned long t)
{
    float s = t / 1000.0f;
    float speed = 1.6f;
    float p = phaseAt(t, speed) * 2.0f;

    float hue = fmodf(300.0f * sinf(s * 0.3f) + 40.0f, 360.0f);

    auto nodeBright = [&](float nodeIndex)
    {
        float d = fabs(p - nodeIndex);
        float b = expf(-3.0f * d);
        b = 0.08f + 0.92f * b;
        return constrain(b, 0.0f, 1.0f);
    };

    float leftB = nodeBright(0.0f);
    float midB = nodeBright(1.0f);
    float rightB = nodeBright(2.0f);

    rgb.SetColor(ColorHSV(hue, 100.0f, (0.5f + 0.5f * midB) * 100.0f).toRGB());

    analogWrite(LED1_PIN, fToByte(leftB));
    analogWrite(LED2_PIN, fToByte(rightB));
}

static float clamp(float v, float a, float b)
{
    return v < a ? a : (v > b ? b : v);
}

void effectReactive(unsigned long t)
{
    float s = t / 1000.0f;
    float randish = 0.5f + 0.5f * sinf(s * 2.3f + 0.7f) * (0.6f + 0.4f * sinf(s * 7.1f));
    float pop = fabs(sinf(s * 3.1f + 0.44f)) > 0.92f ? map(constrain(int(s) % 5, 0, 5), 0, 5, 0.8f, 1.0f) : 1.0f;
    float centerB = constrain(0.25f + 0.75f * randish * pop, 0.05f, 1.0f);

    float hue = fmodf(20.0f + 40.0f * sinf(s * 0.9f) + 120.0f * sinf(s * 0.03f), 360.0f);
    rgb.SetColor(ColorHSV(hue, 90.0f, centerB * 100.0f).toRGB());

    float left = 0.18f + 0.6f * (0.5f + 0.5f * sinf(s * 10.0f + 0.2f));
    float right = 0.18f + 0.6f * (0.5f + 0.5f * sinf(s * 10.0f + 2.1f));
    analogWrite(LED1_PIN, fToByte(constrain(left * (0.6f + 0.4f * randish), 0.0f, 1.0f)));
    analogWrite(LED2_PIN, fToByte(constrain(right * (0.6f + 0.4f * (1.0f - randish)), 0.0f, 1.0f)));
}

void effectStrobeRainbow(unsigned long t)
{
    float s = t / 1000.0f;
    float beat = 8.0f;
    float osc = fabs(sinf(s * beat * M_PI));
    int stepIndex = (int)floor((s * beat) / 4.0f);
    float hueBase = fmodf(fmodf(stepIndex * 60.0f + s * 10.0f, 360.0f) + 360.0f, 360.0f);
    float val = 0.2f + 0.8f * osc;

    rgb.SetColor(ColorHSV(hueBase, 100.0f, val * 100.0f).toRGB());

    analogWrite(LED1_PIN, fToByte(clamp(0.12f + osc * 1.0f, 0.0f, 1.0f)));
    analogWrite(LED2_PIN, fToByte(clamp(0.12f + (1.0f - osc) * 0.9f, 0.0f, 1.0f)));
}

void setup()
{
    rgb.InitializePins();
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    rgb.SetColor(ColorRGB(0, 0, 0));
    prevMillis = millis();
    modeStartMillis = prevMillis;
}

void loop()
{
    unsigned long now = millis();
    if (now - modeStartMillis >= MODE_DURATION)
    {
        modeIndex = (modeIndex + 1) % MODE_COUNT;
        modeStartMillis = now;
    }

    unsigned long t = now - modeStartMillis;
    switch (modeIndex)
    {
    case 0:
        effectAurora(t);
        break;
    case 1:
        effectTriPulse(t);
        break;
    case 2:
        effectColorSwipe(t);
        break;
    case 3:
        effectMeteor(t);
        break;
    case 4:
        effectReactive(t);
        break;
    case 5:
        effectStrobeRainbow(t);
        break;
    default:
        effectAurora(t);
        break;
    }

    delay(10);
}
