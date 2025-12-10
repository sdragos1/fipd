#include "rgb.hpp"
#include "hsl.hpp"
#include "hsv.hpp"

ColorHSL ColorRGB::toHSL() const
{
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;

    float maxC = max(max(rf, gf), bf);
    float minC = min(min(rf, gf), bf);
    float delta = maxC - minC;

    float h = 0, s = 0, l = (maxC + minC) / 2.0f;

    if (delta != 0)
    {
        s = (l < 0.5f) ? (delta / (maxC + minC)) : (delta / (2.0f - maxC - minC));

        if (maxC == rf)
        {
            h = 60.0f * fmod((gf - bf) / delta, 6.0f);
        }
        else if (maxC == gf)
        {
            h = 60.0f * ((bf - rf) / delta + 2.0f);
        }
        else
        {
            h = 60.0f * ((rf - gf) / delta + 4.0f);
        }

        if (h < 0)
            h += 360.0f;
    }

    return ColorHSL(h, s * 100.0f, l * 100.0f);
}

ColorHSV ColorRGB::toHSV() const {
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;

    float maxC = max(max(rf, gf), bf);
    float minC = min(min(rf, gf), bf);
    float delta = maxC - minC;

    float h = 0, s = 0, v = maxC;
    if (delta != 0) {
        if (maxC == rf) h = 60.0f * fmod((gf - bf) / delta, 6.0f);
        else if (maxC == gf) h = 60.0f * ((bf - rf) / delta + 2.0f);
        else h = 60.0f * ((rf - gf) / delta + 4.0f);
        if (h < 0) h += 360.0f;
        s = delta / maxC;
    }
    return ColorHSV(h, s * 100.0f, v * 100.0f);
}

ColorRGB ColorRGB::fromHex(int hex)
{
    return ColorRGB((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
}

int ColorRGB::toHex() const
{
    return ((int)r << 16) | ((int)g << 8) | b;
}

const ColorRGB ColorRGB::kBlack = ColorRGB(0, 0, 0);
const ColorRGB ColorRGB::kWhite = ColorRGB(255, 255, 255);
const ColorRGB ColorRGB::kRed = ColorRGB(255, 0, 0);
const ColorRGB ColorRGB::kGreen = ColorRGB(0, 255, 0);
const ColorRGB ColorRGB::kBlue = ColorRGB(0, 0, 255);
const ColorRGB ColorRGB::kYellow = ColorRGB(255, 255, 0 );
const ColorRGB ColorRGB::kCyan = ColorRGB(0, 255, 255);
const ColorRGB ColorRGB::kMagenta = ColorRGB(255, 0, 255);  
