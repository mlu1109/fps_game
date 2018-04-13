#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include "wavefront_object.hpp"

/* This is a simple (probably bug-ridden) Wavefront Object parser
 * https://en.wikipedia.org/wiki/Wavefront_.obj_file
 * Does only suppot 3d vertices and normals (eventual w skipped) and 2d vertex textures (eventual depth skipped)
 */

/* 
 * String helpers 
 */

// Tokens

bool isEmpty(const std::string &line)
{
    return line.find_first_not_of("\t ") == std::string::npos;
}

bool isComment(const std::string &line)
{
    return line[0] == '#';
}

// FaceElements

bool isSingleFE(const std::string &fe)
{
    return std::find(fe.begin(), fe.end(), '/') == fe.end();
}

/* 
 * Parse args 
 */

// String to glm::vec

glm::vec3 parseVec3(const std::string &args)
{
    glm::vec3 v;
    std::stringstream ss(args);
    ss >> v.x;
    ss >> v.y;
    ss >> v.z;
    // Expecting args to be empty
    std::string trailing;
    std::getline(ss, trailing);
    if (trailing.size())
        std::cout << "(parseVec3) Warning: Expected empty string but got " << trailing << '\n';

    return v;
}

glm::vec2 parseVec2(const std::string &args)
{
    glm::vec2 v;
    std::stringstream ss(args);
    ss >> v.x;
    ss >> v.y;
    // Expecting args to be empty
    std::string trailing;
    std::getline(ss, trailing);
    if (trailing.size())
        std::cout << "(parseVec2) Warning: Expected empty string but got " << trailing << '\n';

    return v;
}

// String to FaceElement

FaceElement parseSingle(const std::string &args)
{
    return FaceElement{std::stoi(args), -1, -1};
}

FaceElement parseTriplet(const std::string &args) // Face element on the form v/vt/vn
{
    // The face element is on the form v/vt/vn
    // v is not allowed to be empty, vt/vn can be an empty string

    FaceElement fe;
    auto delim1 = std::find(args.begin(), args.end(), '/');
    fe.v = std::stoi(std::string(args.begin(), delim1));
    auto delim2 = std::find(delim1 + 1, args.end(), '/');
    if (delim2 == delim1 + 1) // v//vn
        fe.vt = -1;
    else
        fe.vt = std::stoi(std::string(delim1 + 1, delim2));
    if (delim2 == args.end() - 1) // v/vt/
        fe.vn = -1;
    else
        fe.vn = std::stoi(std::string(delim2 + 1, args.end()));

    return fe;
}

/* 
 * Parse lines 
 */

void parseV(const std::string &args, WavefrontObject &result)
{
    result.cntV += 1;
    glm::vec3 v = parseVec3(args);
    result.v.push_back(v);
}

void parseVT(const std::string &args, WavefrontObject &result)
{
    result.cntVT += 1;
    glm::vec2 vt = parseVec2(args);
    result.vt.push_back(vt);
}

void parseVN(const std::string &args, WavefrontObject &result)
{
    result.cntVN += 1;
    glm::vec3 vn = parseVec3(args);
    result.vn.push_back(vn);
}

void parseF(const std::string &args, WavefrontObject &result)
{
    result.cntF += 1;
    std::vector<FaceElement> face;
    std::stringstream ss(args);
    std::string arg;
    std::vector<FaceElement> fes;
    while (ss >> arg)
    {
        result.cntFE += 1;
        FaceElement fe;
        if (isSingleFE(arg))
            fe = parseSingle(arg);
        else
            fe = parseTriplet(arg);
        fes.push_back(fe);
    }
    result.f.push_back(fes);
}

/* 
 * Parse file with path
 */

WavefrontObject parseWavefrontObject(const std::string &path)
{
    WavefrontObject result;

    std::string line;
    std::ifstream file(path);

    while (std::getline(file, line))
    {
        if (isEmpty(line) || isComment(line))
            continue;

        auto separator = std::find(line.begin(), line.end(), ' ');
        std::string token(line.begin(), separator);
        std::string args(separator, line.end());

        if (token == "v")
        {
            parseV(args, result);
        }
        else if (token == "vt")
        {
            parseVT(args, result);
        }
        else if (token == "vn")
        {
            parseVN(args, result);
        }
        else if (token == "f")
        {
            parseF(args, result);
        }
        else
        {
            std::cout << "(parse) Warning: Skipping unhandled token " << token << '\n';
        }
    }

    return result;
}