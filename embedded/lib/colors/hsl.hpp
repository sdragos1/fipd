#pragma once

#include <Arduino.h>

class ColorRGB;

class ColorHSL
{
public:
    float h, s, l;

    ColorHSL(float hue = 0, float saturation = 0, float lightness = 0);

    static ColorHSL fromHue(float hue) { return ColorHSL(hue, 100, 50); }

    ColorRGB toRGB() const;
};