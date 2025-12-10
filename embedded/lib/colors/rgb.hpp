#pragma once

#include <Arduino.h>

class ColorHSL;
class ColorHSV;

class ColorRGB
{
public:
    int r, g, b;

    ColorRGB(int red = 0, int green = 0, int blue = 0)
        : r(red), g(green), b(blue) {}

    static ColorRGB fromHex(int hex);

    int toHex() const;

    ColorHSL toHSL() const;

    ColorHSV toHSV() const;

    static const ColorRGB kBlack;
    static const ColorRGB kWhite;
    static const ColorRGB kRed;
    static const ColorRGB kGreen;
    static const ColorRGB kBlue;
    static const ColorRGB kYellow;
    static const ColorRGB kCyan;
    static const ColorRGB kMagenta;
};