#include "Image.h"
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

unsigned char Image::clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return static_cast<unsigned  char>(value);
}

unsigned char Image::roundFloat(float value) {
    int result = static_cast<int>(value + 0.5f);
    return clamp(result);
}

// read/write functions
bool Image::read(const string &filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return false;

    file.read(reinterpret_cast<char*>(&header), sizeof(Header));
    int dataSize = header.width * header.height * 3;
    pixels.resize(dataSize);
    file.read(reinterpret_cast<char*>(pixels.data()), dataSize);
    file.close();
    return true;
}

bool Image::write(const string &filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return false;

    file.write(reinterpret_cast<const char*>(&header), sizeof(Header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    file.close();
    return true;
}

// Image operation funtions
Image Image::multiply(const Image &top, const Image &bottom) {
    Image result = bottom;
    for (size_t i = 0; i < result.pixels.size(); i++) {
        result.pixels[i] = roundFloat(((top.pixels[i] / 255.0f) * (bottom.pixels[i] / 255.0f)) * 255.0f);
    }
    return result;
}

Image Image::screen(const Image &top, const Image &bottom) {
    Image result = bottom;
    for (size_t i = 0; i < result.pixels.size(); i++) {
        float nt = top.pixels[i] / 255.0f;
        float nb = bottom.pixels[i] / 255.0f;
        result.pixels[i] = roundFloat((1 - (1 - nt) * (1 - nb)) * 255.0f);
    }
    return result;
}

Image Image::subtract(const Image &top, const Image &bottom) {
    Image result = bottom;
    for (size_t i = 0; i < result.pixels.size(); i++) {
        result.pixels[i] = clamp((int)bottom.pixels[i] - (int)top.pixels[i]);
    }
    return result;
}

Image Image::add(const Image &top, const Image &bottom) {
    Image result = bottom;
    for (size_t i = 0; i < result.pixels.size(); i++) {
        result.pixels[i] = clamp((int)bottom.pixels[i] + (int)top.pixels[i]);
    }
    return result;
}

Image Image::overlay(const Image &top, const Image &bottom) {
    Image result = bottom;
    for (size_t i = 0; i < result.pixels.size(); i++) {
        float nt = top.pixels[i] / 255.0f;
        float nb = bottom.pixels[i] / 255.0f;
        float val = (nb <= 0.5f) ? (2 * nt * nb) : (1 - 2 * (1 - nt) * (1 - nb));
        result.pixels[i] = roundFloat(val * 255.0f);
    }
    return result;
}

// channel editing functions
Image Image::addChannel(const Image &img, int r, int g, int b) {
    Image result = img;
    for (size_t i = 0; i < result.pixels.size(); i += 3) {
        result.pixels[i] = clamp(static_cast<int>(result.pixels[i] * b));
        result.pixels[i+1] = clamp(static_cast<int>(result.pixels[i+1] * g));
        result.pixels[i+2] = clamp(static_cast<int>(result.pixels[i+2] * r));
    }
    return result;
}

Image Image::scaleChannel(const Image &img, float r, float g, float b) {
    Image result = img; 
    for (size_t i = 0; i < result.pixels.size(); i += 3) {
        result.pixels[i] = clamp(static_cast<int>(result.pixels[i] * b));
        result.pixels[i+1] = clamp(static_cast<int>(result.pixels[i+1] * g));
        result.pixels[i+2] = clamp(static_cast<int>(result.pixels[i+2] * r));
    }
    return result;
}

void Image::splitChannels(const Image &img, Image &r, Image &g, Image &b) {
    r = img; g = img; b = img;
    for (size_t i = 0; i < img.pixels.size(); i +=3) {
        unsigned char blue = img.pixels[i];
        unsigned char green = img.pixels[i+1];
        unsigned char red = img.pixels[i+2];
        r.pixels[i] = r.pixels[i+1] = r.pixels[i+2] = red;
        g.pixels[i] = g.pixels[i+1] = g.pixels[i+2] = green;
        b.pixels[i] = b.pixels[i+1] = b.pixels[i+2] = blue;
    }
}

Image Image::combine(const Image &r, const Image &g, const Image &b) {
    Image result = r;
    for (size_t i = 0; i < result.pixels.size(); i+=3) {
        result.pixels[i] = b.pixels[i];
        result.pixels[i+1] = g.pixels[i];
        result.pixels[i+2] = r.pixels[i];
    }
    return result;
}

Image Image::rotate180(const Image &img) {
    Image result = img;
    size_t total = img.pixels.size();
    for (size_t i = 0; i < total; i+=3) {
        result.pixels[i] = img.pixels[total - 3 - i];
        result.pixels[i+1] = img.pixels[total - 2 - i];
        result.pixels[i+2] = img.pixels[total - 1 - i];
    }
    return result;
}