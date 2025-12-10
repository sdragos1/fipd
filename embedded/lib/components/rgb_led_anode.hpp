#pragma once
#include <Arduino.h>
#include <rgb.hpp>
#include <hsl.hpp>

class RGBLedAnode
{
public:
    RGBLedAnode(const int redPin = 9, const int greenPin = 10, const int bluePin = 11)
        : kRedPin(redPin), kGreenPin(greenPin), kBluePin(bluePin) {};

    ~RGBLedAnode();

    void InitializePins();
    void SetColor(ColorRGB color);
    void SetColor(ColorHSL color);

private:
    int kRedPin = 9;
    int kGreenPin = 10;
    int kBluePin = 11;
};
