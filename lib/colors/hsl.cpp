#include "hsl.hpp"
#include "rgb.hpp"

static float hueToRGB(float p, float q, float t) {
    if (t < 0) t += 1.0f;
    if (t > 1) t -= 1.0f;
    if (t < 1.0f/6.0f) return p + (q - p) * 6.0f * t;
    if (t < 1.0f/2.0f) return q;
    if (t < 2.0f/3.0f) return p + (q - p) * (2.0f/3.0f - t) * 6.0f;
    return p;
}

ColorHSL::ColorHSL(float hue, float saturation, float lightness)
    : h(hue), s(saturation), l(lightness) {}

ColorRGB ColorHSL::toRGB() const
{
    float hNorm = h / 360.0f;
    float sNorm = s / 100.0f;
    float lNorm = l / 100.0f;
    
    if (sNorm == 0) {
        uint8_t gray = lNorm * 255;
        return ColorRGB(gray, gray, gray);
    }
    
    float q = (lNorm < 0.5f) ? (lNorm * (1.0f + sNorm)) : (lNorm + sNorm - lNorm * sNorm);
    float p = 2.0f * lNorm - q;
    
    float r = hueToRGB(p, q, hNorm + 1.0f/3.0f);
    float g = hueToRGB(p, q, hNorm);
    float b = hueToRGB(p, q, hNorm - 1.0f/3.0f);
    
    return ColorRGB(r * 255, g * 255, b * 255);
}