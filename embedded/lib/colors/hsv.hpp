#pragma once

#include <Arduino.h>

class ColorRGB;

class ColorHSV {
public:
    float h;
    float s;
    float v;

    ColorHSV(float hue = 0, float saturation = 0, float value = 0)
        : h(hue), s(saturation), v(value) {}

    ColorRGB toRGB() const;
};
