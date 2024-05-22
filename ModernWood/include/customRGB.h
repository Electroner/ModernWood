#pragma once

// RGB color struct
class RGB
{
public:
    int r, g, b; // RGB
    int color;  // 16-bit color
    float h, s, v; // HSV HUE, SATURATION, VALUE

    RGB() : r(0), g(0), b(0), color(0), h(0.0f), s(1.0f), v(1.0f) {}

    RGB(int red, int green, int blue)
    {
        r = red;
        g = green;
        b = blue;
        s = 1.0f;
        v = 1.0f;
        CalculateColor();
        rgbToHsv();
    }

    RGB(int color)
    {
        this->color = color;
        CalculateRGB();
        rgbToHsv();
    }

    RGB(float h, float s, float v)
    {
        this->h = h;
        this->s = s;
        this->v = v;
        hsvToRgb();
        CalculateColor();
    }

    void CalculateColor()
    {
        color = (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);
    }

    void CalculateRGB()
    {
        r = ((color >> 11) & 0x1F) << 3;
        g = ((color >> 5) & 0x3F) << 2;
        b = (color & 0x1F) << 3;
    }

    void hsvToRgb()
    {
        float red, green, blue;
        int i = static_cast<int>(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        switch (i % 6) {
            case 0: red = v; green = t; blue = p; break;
            case 1: red = q; green = v; blue = p; break;
            case 2: red = p; green = v; blue = t; break;
            case 3: red = p; green = q; blue = v; break;
            case 4: red = t; green = p; blue = v; break;
            case 5: red = v; green = p; blue = q; break;
        }

        r = static_cast<int>(red * 255);
        g = static_cast<int>(green * 255);
        b = static_cast<int>(blue * 255);
    }

    void rgbToHsv()
    {
        float rf = r / 255.0f;
        float gf = g / 255.0f;
        float bf = b / 255.0f;

        float max = std::max(rf, std::max(gf, bf));
        float min = std::min(rf, std::min(gf, bf));
        float delta = max - min;

        v = max;

        if (max != 0)
            s = delta / max;
        else {
            s = 0;
            h = 0;
            return;
        }

        if (delta == 0)
            h = 0;
        else if (max == rf)
            h = (gf - bf) / delta;
        else if (max == gf)
            h = 2 + (bf - rf) / delta;
        else
            h = 4 + (rf - gf) / delta;

        h *= 60;
        if (h < 0)
            h += 360;
    }
};