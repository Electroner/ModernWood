#pragma once

//RGB color struct
class RGB {
public:
    int r, g, b;
    int color;
    RGB(int red, int green, int blue){
        r = red;
        g = green;
        b = blue;
        CalculateColor();
    }
    void CalculateColor() {
        color = r << 16 | g << 8 | b;
        //color = (r * 65536) + (g * 256) + b;
    }
    void CalculateRGB(){
        r = (color >> 16) & 0xFF;
        g = (color >> 8) & 0xFF;
        b = color & 0xFF;
        //r = color / 65536;
        //g = (color / 256) % 256;
        //b = color % 256;
    }
};