#include <algorithm>
#include <exception>
#include <fstream>
#include <sstream>
#include <iostream>
#include "wavefront_object.hpp"

/* 
 * safeGetline copied from answer to question: https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
 * Credit: https://stackoverflow.com/users/763305/user763305
 */

std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

/* 
 * String helpers 
 */

// Tokens

bool isEmpty(const std::string &line)
{
    return line.find_first_not_of("\t\r\0\n ") == std::string::npos;
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
    safeGetline(ss, trailing);
    if (!isEmpty(trailing))
        std::cout << "(parseVec3) Warning: Expected empty string but got\n'" << trailing << "'\n";

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
    safeGetline(ss, trailing);
    if (!isEmpty(trailing))
        std::cout << "(parseVec2) Warning: Expected empty string but got\n '" << trailing << "'\n";

    return v;
}

// String to FaceElement

FaceElement parseSingle(const std::string &args)
{
    return FaceElement{std::stoi(args), 0, 0}; // .obj files have 1-based array indices.
}

FaceElement parseTriplet(const std::string &args)
{
    // The face element is on the form v/vt/vn
    // v is not allowed to be empty, vt/vn can be an empty string

    FaceElement fe;
    auto delim1 = std::find(args.begin(), args.end(), '/');
    fe.v = std::stoi(std::string(args.begin(), delim1));
    auto delim2 = std::find(delim1 + 1, args.end(), '/');
    if (delim2 == delim1 + 1) // v//vn
        fe.vt = 0; // .obj files have 1-based array indices.
    else
        fe.vt = std::stoi(std::string(delim1 + 1, delim2));
    if (delim2 == args.end() - 1) // v/vt/
        fe.vn = 0; // .obj files have 1-based array indices.
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

    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);

    while (safeGetline(file, line))
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