#ifndef COLOR_H
#define COLOR_H

typedef unsigned char uchar;

class Color
{
private:
    float trunc(float x)
    {
        if (x < 0.0) return 0.0;
        else if (x > 1.0) return 1.0;
        return x;
    }

public:
    float r;
    float g;
    float b;
    float a;

    Color(float R, float G, float B, float A = 1.0) {
        r = trunc(R);
        g = trunc(G);
        b = trunc(B);
        a = trunc(A);
    }

    Color(int C) {
        r = (C & 0xFF)         / 255.0;
        g = ((C >>  8) & 0xFF) / 255.0;
        b = ((C >> 16) & 0xFF) / 255.0;
        a = ((C >> 24) & 0xFF) / 255.0;
    }

    Color() {
        r = g = b = 0.0;
        a = 1.0;
    }

    int toInt() {
        return int(r*255) + (int(g*255) << 8) + (int(b*255) << 16) + (int(a*255) << 24);
    }



    Color mix(Color x) {
        return Color(trunc(x.r * x.a + this->r * (1.0 - x.a)),
                     trunc(x.g * x.a + this->g * (1.0 - x.a)),
                     trunc(x.b * x.a + this->b * (1.0 - x.a)),
                     trunc(this->a + x.a));
    }

    static Color blue() {
        return Color(0.0, 0.0, 1.0);
    }

    static Color red() {
        return Color(1.0, 0.0, 0.0);
    }

    static Color green() {
        return Color(0.0, 1.0, 0.0);
    }

    static Color black() {
        return Color(0.0, 0.0, 0.0);
    }
};

#endif // COLOR_H
