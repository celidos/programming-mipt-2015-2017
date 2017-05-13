#ifndef COLOR_H
#define COLOR_H

typedef unsigned char uchar;

class Color
{
private:
    float truncf(float x)
    {
        if (x < 0.0) return 0.0;
        else if (x > 1.0) return 1.0;
        return x;
    }

    unsigned int trunci(unsigned int x)
    {
        if (x > 255)
            return 255;
        return x;
    }

public:
    float r;
    float g;
    float b;
    float a;

    Color(float R, float G, float B, float A = 1.0) {
        r = truncf(R);
        g = truncf(G);
        b = truncf(B);
        a = truncf(A);
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

    Color from3chars(unsigned int R, unsigned int G, unsigned int B) {
        r = trunci(R) * 1.0 / 255.0;
        g = trunci(G) * 1.0 / 255.0;
        b = trunci(B) * 1.0 / 255.0;
        return *this;
    }

    Color setAchar(unsigned int A)
    {
        a = trunci(A) * 1.0 / 255.0;
        return *this;
    }

    Color mix(Color x) {
        return Color(truncf(x.r * x.a + this->r * (1.0 - x.a)),
                     truncf(x.g * x.a + this->g * (1.0 - x.a)),
                     truncf(x.b * x.a + this->b * (1.0 - x.a)),
                     truncf(this->a + x.a));
    }

    Color brighter(float scale) {
        r = truncf(r + scale);
        g = truncf(g + scale);
        b = truncf(b + scale);
        return *this;
    }

    Color less(float scale) {
        r = truncf(r * scale);
        g = truncf(g * scale);
        b = truncf(b * scale);
        return *this;
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

    static Color yellow() {
        return Color(1.0, 1.0, 0.0);
    }
};

#endif // COLOR_H
