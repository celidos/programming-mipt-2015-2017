#ifndef COLOR_H
#define COLOR_H

#include "debug_functional.h"

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

    Color set3i(unsigned int R, unsigned int G, unsigned int B) {
        r = trunci(R) * 1.0 / 255.0;
        g = trunci(G) * 1.0 / 255.0;
        b = trunci(B) * 1.0 / 255.0;
        return *this;
    }

    Color setAi(unsigned int A)
    {
        a = trunci(A) * 1.0 / 255.0;
        return *this;
    }

    Color setAf(float A)
    {
        a = truncf(A);
        return *this;
    }

    Color mix(Color x, float aa) {
        return Color(truncf(x.r * aa + this->r * (1.0 - aa)),
                     truncf(x.g * aa + this->g * (1.0 - aa)),
                     truncf(x.b * aa + this->b * (1.0 - aa)),
                     truncf(this->a + aa));
    }

    Color rise(float scale) {
        return Color(truncf(r + scale),
                     truncf(g + scale),
                     truncf(b + scale));
    }

    Color scale(float scale) {
        return Color(truncf(r * scale), truncf(g * scale), truncf(b * scale));
    }

    Color add(Color other) {
        return Color(truncf(r + other.r),
                     truncf(g + other.g),
                     truncf(b + other.b));
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

    std::string toStr() {
        return "(" + toString(r) + "; " + toString(g) + "; " + toString(b) + "; a = " + toString(a) + ")";
    }
};


#endif // COLOR_H
