#pragma once

// RGB color struct
class RGB
{
public:
    int r, g, b;
    int color;
    RGB() {}
    RGB(int red, int green, int blue)
    {
        r = red;
        g = green;
        b = blue;
        CalculateColor();
    }
    RGB(int color)
    {
        this->color = color;
        CalculateRGB();
    }
    void CalculateColor()
    {
        color = (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);
        // color = (r * 65536) + (g * 256) + b;
    }
    void CalculateRGB()
    {
        r = ((color >> 11) & 0x1F) << 3;
        g = ((color >> 5) & 0x3F) << 2;
        b = (color & 0x1F) << 3;
        // r = color / 65536;
        // g = (color / 256) % 256;
        // b = color % 256;
    }
};