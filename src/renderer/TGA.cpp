#include <algorithm>
#include <fstream>
#include <iostream>
#include "TGA.hpp"

/* NOTE: Does not handle all types of TGA */

/* 
 * Used resources:
 * http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga's/22001/
 * http://www.paulbourke.net/dataformats/tga/
 */

TGA readHeader(std::istream &file, const std::string &path)
{
    TGA tga;

    std::array<unsigned char, 6> &header = tga.header;
    file.read(reinterpret_cast<char *>(header.data()), header.size());
    if (!file.good())
        throw std::runtime_error("Unexpected error while reading file: " + path);

    tga.width = header[1] * 256 + header[0];
    tga.height = header[3] * 256 + header[2];
    tga.bitsPerPixel = header[4];
    tga.bytesPerPixel = header[4] / 8;

    /* Validate */
    if (tga.width <= 0 || tga.height <= 0 ||
        (tga.bitsPerPixel != 24 && tga.bitsPerPixel != 32))
    {
        throw std::runtime_error("Unhandled TGA type: " + path);
    }

    if (tga.bitsPerPixel == 24)
        tga.type = TGA::RGB;
    else // 32
        tga.type = TGA::RGBA;

    tga.imageSize = tga.bytesPerPixel * tga.width * tga.height;

    return tga;
}

TGA loadUncompressedTGA(std::istream &file, const std::string &path)
{
    TGA tga = readHeader(file, path);
    tga.imageData = std::vector<unsigned char>(tga.imageSize);
    file.read(reinterpret_cast<char *>(tga.imageData.data()), tga.imageData.size());
    if (!file.good())
        throw std::runtime_error("Unexpected error while reading file: " + path);

    return tga;
}

TGA loadCompressedTGA(std::istream &file, const std::string &path)
{
    TGA tga = readHeader(file, path);
    tga.imageData = std::vector<unsigned char>(tga.imageSize);

    /* Decompress */
    int currentPixel = 0;
    int pixelCount = tga.width * tga.height;
    while (currentPixel < pixelCount)
    {
        /* Get chunkHeader */
        unsigned char chunkHeader = 0;
        chunkHeader = static_cast<unsigned char>(file.get());
        if (file.gcount() != 1)
            throw std::runtime_error("Unexpected error while reading file: " + path);

        /* Handle section */
        if (chunkHeader <= 127) // Section is raw
        {
            chunkHeader += 1; // Number of pixels in section
            std::vector<unsigned char> sectionBuffer(chunkHeader * tga.bytesPerPixel);
            file.read(reinterpret_cast<char *>(sectionBuffer.data()), sectionBuffer.size());
            if (!file.good())
                throw std::runtime_error("Unexpected error while reading file: " + path);

            std::move(sectionBuffer.begin(), sectionBuffer.end(), tga.imageData.begin() + currentPixel * tga.bytesPerPixel);
        }
        else // Section is run-length encoded
        {
            chunkHeader -= 127; // Number of times next pixel is repeated
            std::vector<unsigned char> pixelBuffer(tga.bytesPerPixel);
            file.read(reinterpret_cast<char *>(pixelBuffer.data()), tga.bytesPerPixel);
            if (!file.good())
                throw std::runtime_error("Unexpected error while reading file: " + path);

            for (auto i = 0; i < chunkHeader; ++i)
                std::copy(pixelBuffer.begin(), pixelBuffer.end(), tga.imageData.begin() + (currentPixel + i) * tga.bytesPerPixel);
        }

        currentPixel += chunkHeader;
    }

    return tga;
}

void bgrToRGB(TGA &tga)
{
    for (int i = 0; i < tga.imageSize; i += tga.bytesPerPixel)
        std::swap(tga.imageData[i], tga.imageData[i + 2]);
}

void flipTGA(TGA &tga)
{
    int row_a = 0;
    int row_b = tga.height - 1;
    while (row_a <= row_b)
    {
        for (int c = 0; c < tga.width; ++c)
        {
            int col_a = c;
            int col_b = tga.width - 1 - c;
            for (int i = 0; i < tga.bytesPerPixel; ++i)
            {
                int i_a = (row_a * tga.width + col_a) * tga.bytesPerPixel;
                int i_b = (row_b * tga.width + col_b) * tga.bytesPerPixel;
                std::swap(tga.imageData[i_a + i], tga.imageData[i_b + i]);
            }
        }

        ++row_a;
        --row_b;
    }
}

TGA loadTGA(const std::string &path)
{
    const std::array<unsigned char, 12> uncompressedTGAHeader = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const std::array<unsigned char, 12> compressedTGAHeader = {0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);

    std::array<unsigned char, 12> header;
    file.read(reinterpret_cast<char *>(header.data()), header.size());
    if (!file.good())
        throw std::runtime_error("Unexpected error while reading file: " + path);

    TGA tga;
    if (header == uncompressedTGAHeader)
        tga = loadUncompressedTGA(file, path);
    else if (header == compressedTGAHeader)
        tga = loadCompressedTGA(file, path);
    else
        throw std::runtime_error("Unhandled TGA header in file: " + path);

    bgrToRGB(tga);
    flipTGA(tga);

    return tga;
}