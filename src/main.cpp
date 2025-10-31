#include "Image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
using namespace std;

//helper functions
static bool ends_with_tga(const string &s) {
    if (s.size() < 4) return false;
    return s.substr(s.size()-4) == ".tga";
}

static bool file_exists(const string &path) {
    ifstream f(path.c_str(), ios::binary);
    return f.is_open();
}

static pair<bool, int> parse_int(const string &s) {
    try {
        size_t idx = 0;
        int v = stoi(s, &idx);
        if (idx != s.size()) return {false, 0};
        return {true, v};
    } catch(...) {
        return {false, 0};
    }
}

static pair<bool, int> parse_scale_int(const string &s) {
    return parse_int(s);
}

static unsigned char local_clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return static_cast<unsigned char>(value);
}

static Image subtract_top_minus_bottom(const Image &topImg, const Image &bottomImg) {
    Image result = topImg;
    size_t sz = result.pixels.size();

    for (size_t i = 0; i < sz; i++) {
        int v = (int)topImg.pixels[i] - (int)bottomImg.pixels[i];
        result.pixels[i] = local_clamp(v);
    }
    return result;
}

static Image subtract_images(const Image &topImg, const Image &bottomImg) {
    Image result = topImg;
    size_t sz = result.pixels.size();
    for (size_t i = 0; i < sz; i++) {
        int v = (int)topImg.pixels[i] - (int)bottomImg.pixels[i];
        result.pixels[i] = local_clamp(v);
    }
    return result;
}

int main(int argc, char** argv) {
    if (argc == 1 || (argc >= 2 && strcmp(argv[1], "--help") == 0)) {
        cout << "Project 2: Image Processing, Fall 2025\n\n";
        cout << "Usage:\n";
        cout << "\t./project2.out [output] [firstImage] [method] [...]\n";
        return 0;
    }

    if (argc < 2) {
        cout << "Invalid File name." << endl;
        return 0;
    }

    string outFilename = argv[1];
    if(!ends_with_tga(outFilename)) {
        cout << "Invalid file name." << endl;
        return 0;
    }

    if (argc < 3) {
        cout << "Invalid file name." << endl;
        return 0;
    }

    string firstImageName = argv[2];
    if (!ends_with_tga(firstImageName)) {
        cout << "Invalid file name." << endl;
        return 0;
    }
    if (!file_exists(firstImageName)) {
        cout << "File does not exist." << endl;
        return 0;
    }

    Image tracking;
    if (!tracking.read(firstImageName)) {
        cout << "File does not exist." << endl;
        return 0;
    }

    int i = 3;
    while (i < argc) {
        string method = argv[i];

        if (method == "multiply" || method == "overlay" || method == "subtract" || method == "screen") {
            if (i + 1 >= argc) {
                cout << "Missing argument." << endl;
                return 0;
            }
            string argfile = argv[i+1];
            if (!ends_with_tga(argfile)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            if (!file_exists(argfile)) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            Image other;
            if (!other.read(argfile)) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            if (method == "multiply") {
                tracking = Image::multiply(tracking, other);
            } else if (method == "subtract") {
                tracking = subtract_images(tracking, other);
            } else {
                tracking = Image::screen(other, tracking);
            }

            i += 2;
            continue;
        }

        if (method == "combine") {
            if (i + 2 >= argc) {
                cout << "Missing argument." << endl;
                return 0;
            }
            string gfile = argv[i+1];
            string bfile = argv[i+2];
            if (!ends_with_tga(gfile) || !ends_with_tga(bfile)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            if (!file_exists(gfile) || !file_exists(bfile)) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            Image gimg, bimg;
            if (!gimg.read(gfile) || !bimg.read(bfile)) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            tracking = Image::combine(tracking, gimg, bimg);
            i += 3;
            continue;
        }

        if (method == "flip") {
            tracking = Image::rotate180(tracking);
            i += 1;
            continue;
        }

        if (method == "onlyred" || method == "onlygreen" || method == "onlyblue") {
            Image r, g, b;
            Image::splitChannels(tracking, r, g, b);
            if (method == "onlyred") tracking = r;
            else if (method == "onlygreen") tracking = g;
            else tracking = b;
            i += 1;
            continue;
        }

        if (method == "addred" || method == "addgreen" || method == "addblue") {
            if (i + 1 >= argc) {
                cout << "Missing argument." << endl;
                return 0;
            }
            auto parsed = parse_int(string(argv[i+1]));
            if (!parsed.first) {
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            int val = parsed.second;

            if (method == "addred") {
                tracking = Image::addChannel(tracking, 0, 0, val);
            } else if (method == "addgreen") {
                tracking = Image::addChannel(tracking, 0, val, 0);
            }else {
                tracking = Image::addChannel(tracking, val, 0, 0);
            }
            i += 2;
            continue;
        }

        if (method == "scalered" || method == "scalegreen" || method == "scaleblue") {
            if (i + 1 >= argc) {
                cout << "Missing argument." << endl;
                return 0;
            }
            auto parsed = parse_scale_int(string(argv[i+1]));
            if (!parsed.first) {
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            int factor = parsed.second;
            if (method == "scalered") {
                tracking = Image::scaleChannel(tracking, (float)factor, 1.0f, 1.0f);
            }else if (method == "scalegreen") {
                tracking = Image::scaleChannel(tracking, 1.0f, (float)factor, 1.0f);
            }else {
                tracking = Image::scaleChannel(tracking, 1.0f, 1.0f, (float)factor);
            }
            i += 2;
            continue;
        }

        cout << "Invalid method name." << endl;
        return 0;
    }
    if (!tracking.write(outFilename)) {
        cout << "Invalid file name." << endl;
        return 0;
    }

    return 0;
}