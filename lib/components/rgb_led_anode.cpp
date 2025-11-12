#include "rgb_led_anode.hpp"

RGBLedAnode::~RGBLedAnode() {}

void RGBLedAnode::InitializePins()
{
    pinMode(kRedPin, OUTPUT);
    pinMode(kGreenPin, OUTPUT);
    pinMode(kBluePin, OUTPUT);
}

void RGBLedAnode::SetColor(ColorRGB color)
{
    analogWrite(kRedPin, 255 - color.r);
    analogWrite(kGreenPin, 255 - color.g);
    analogWrite(kBluePin, 255 - color.b);
}

void RGBLedAnode::SetColor(ColorHSL color)
{
    SetColor(color.toRGB());
}
