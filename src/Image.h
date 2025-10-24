#pragma once
#include "TGA.h"
#include <vector>
#include <string>
using namespace std;
// actual image stuff here

class Image {
public:
    Header header;
    vector<unsigned char> pixels; // stored as BGR becuase why tf not

    // load from or write to stuff (prolly wrong, review with a TA or someone good at this shit)
    bool read(const string &filename);
    bool write(const string &filename) const;

    // pixel manipulation algorithms here, get your pixel manipulation algorithms here 
    static Image multiply(const Image &top, const Image &bottom);
    static Image screen(const Image &top, const Image &bottom);
    static Image subtract(const Image &top, const Image &bottom);
    static Image add(const Image &top, const Image &bottom);
    static Image overlay(const Image &top, const Image &bottom);

    // channel modification utilities
    static Image addChannel(const Image &img, int r, int g, int b);
    static Image scaleChannel(const Image &img, float r, float g, float b);
    static void splitChannels(const Image &img, Image &r, Image &g, Image &b);
    static Image combine(const Image &r, const Image &g, const Image &b);

    // rotate image 180degrees
    static Image rotate180(const Image &img);

private:
    // helper functions for rounding and clamping pixel values
    static unsigned char clamp(int value);
    static unsigned char roundFloat(float value);
};