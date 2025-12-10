#include "hsv.hpp"
#include "rgb.hpp"

ColorRGB ColorHSV::toRGB() const {
    float hh = fmodf(h, 360.0f);
    if (hh < 0) hh += 360.0f;
    float s_n = s / 100.0f;
    float v_n = v / 100.0f;

    float c = v_n * s_n;
    float x = c * (1 - fabsf(fmodf(hh / 60.0f, 2.0f) - 1));
    float m = v_n - c;
    float rf = 0, gf = 0, bf = 0;

    if (hh < 60) { rf = c; gf = x; bf = 0; }
    else if (hh < 120) { rf = x; gf = c; bf = 0; }
    else if (hh < 180) { rf = 0; gf = c; bf = x; }
    else if (hh < 240) { rf = 0; gf = x; bf = c; }
    else if (hh < 300) { rf = x; gf = 0; bf = c; }
    else { rf = c; gf = 0; bf = x; }

    return ColorRGB((int)roundf((rf + m) * 255.0f), (int)roundf((gf + m) * 255.0f), (int)roundf((bf + m) * 255.0f));
}
