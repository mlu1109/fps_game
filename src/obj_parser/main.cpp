#include "wavefront_object.hpp"
#include <iostream>

int main()
{
    const std::string path = "src/assets/models/tsbk07/teapot.obj";
    WavefrontObject octagon = parseWavefrontObject(path);
}