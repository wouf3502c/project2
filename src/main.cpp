#include "Image.h"
#include <iostream>
#include <string>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
using namespace std;

int main() {
#ifdef _WIN32
    _mkdir("output");
#else 
    mkdir("output", 0777);
#endif

    // load everything
    Image layer1, layer2, pattern1, pattern2, text, car, circles;
    layer1.read("input/layer1.tga");
    layer2.read("input/layer2.tga");
    pattern1.read("input/pattern1.tga");
    pattern2.read("input/pattern2.tga");
    text.read("input/text.tga");
    car.read("input/car.tga");
    circles.read("input/circles.tga");

    // task 1
    Image part1 = Image::multiply(layer1, pattern1);
    part1.write("output/part1.tga");

    // task 2
    Image part2 = Image::subtract(car, layer2);
    part2.write("output/part2.tga");

    // task 3
    Image temp3 = Image::multiply(layer1, pattern2);
    Image part3 = Image::screen(text, temp3);
    part3.write("output/part3.tga");

    // task 4
    Image temp4 = Image::multiply(layer2, circles);
    Image part4 = Image::subtract(temp4, pattern2);
    part4.write("output/part4.tga");

    // task 5
    Image part5 = Image::overlay(layer1, pattern1);
    part5.write("output/part5.tga");

    // task 6
    Image part6 = Image::addChannel(car, 0, 200, 0);
    part6.write("output/part6.tga");

    // task 7
    Image part7 = Image::scaleChannel(car, 4, 1, 0);
    part7.write("output/part7.tga");

    // task 8
    Image red, green, blue;
    Image::splitChannels(car, red, green, blue);
    red.write("output/part8_r.tga");
    green.write("output/part8_g.tga");
    blue.write("output/part8_b.tga");

    // task 9
    Image layerR, layerG, layerB;
    layerR.read("input/layer_red.tga");
    layerG.read("input/layer_green.tga");
    layerB.read("input/layer_blue.tga");
    Image part9 = Image::combine(layerR, layerG, layerB);
    part9.write("output/part9.tga");

    // task 10
    Image text2;
    text2.read("input/text.tga");
    Image part10 = Image::rotate180(text2);
    part10.write("output/part10.tga");

    cout << "All 10 tasks completed successfully. \n";
    return 0;
}
