#pragma once

#include <GL/glew.h>
#include <array>
#include <vector>

struct TGA
{
    // Used by type
    const static int RGB = 2;
    const static int RGBA = 1;

    std::array<unsigned char, 6> header;
    std::vector<unsigned char> imageData;
    int bytesPerPixel = 0;
    int imageSize = 0;
    int type = 0; // TGA::RGB / TGA::RGBA
    int height = 0;
    int width = 0;
    int bitsPerPixel = 0;
};

TGA loadTGA(const std::string &path);