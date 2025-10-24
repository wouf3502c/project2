#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#pragma pack(push, 1)
struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};
#pragma pack(pop)