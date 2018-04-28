#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include "OBJ.hpp"

/* 
 * safeGetline copied from answer to question: https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
 * Credit: https://stackoverflow.com/users/763305/user763305
 */

std::istream &safeGetline(std::istream &is, std::string &t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf *sb = is.rdbuf();

    for (;;)
    {
        int c = sb->sbumpc();
        switch (c)
        {
        case '\n':
            return is;
        case '\r':
            if (sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if (t.empty())
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
        fe.vt = 0;            // .obj files have 1-based array indices.
    else
        fe.vt = std::stoi(std::string(delim1 + 1, delim2));
    if (delim2 == args.end() - 1) // v/vt/
        fe.vn = 0;                // .obj files have 1-based array indices.
    else
        fe.vn = std::stoi(std::string(delim2 + 1, args.end()));

    return fe;
}

/* 
 * Parse lines 
 */

void OBJ::parseV(const std::string &args)
{
    glm::vec3 v = parseVec3(args);
    m_v.push_back(v);
}

void OBJ::parseVT(const std::string &args)
{
    glm::vec2 vt = parseVec2(args);
    m_vt.push_back(vt);
}

void OBJ::parseVN(const std::string &args)
{
    glm::vec3 vn = parseVec3(args);
    m_vn.push_back(vn);
}

void OBJ::parseF(const std::string &args)
{
    std::vector<FaceElement> face;
    std::stringstream ss(args);
    std::string arg;
    std::vector<FaceElement> fes;
    while (ss >> arg)
    {
        FaceElement fe;
        if (isSingleFE(arg))
            fe = parseSingle(arg);
        else
            fe = parseTriplet(arg);
        fes.push_back(fe);
    }
    m_f.push_back(fes);
}

/*
 * Entry
 */

OBJ::OBJ(const std::string &path)
{
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
            parseV(args);
        }
        else if (token == "vt")
        {
            parseVT(args);
        }
        else if (token == "vn")
        {
            parseVN(args);
        }
        else if (token == "f")
        {
            parseF(args);
        }
        else
        {
            std::cout << "(parse) Warning: Skipping unhandled token " << token << '\n';
        }
    }
}

/*
 * Other
 */

std::pair<std::vector<Vertex>, std::vector<GLuint>> OBJ::getVerticesIndices() const
{
    // Because OBJ face elements and vertex/texcoord/normal triplets are not guaranteed to have a
    // one-to-one mapping new vertex/texcoord/normal triplets might need to be generated.
    std::map<Vertex, GLuint> vertexToIndex;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (const auto &face : m_f)
    {
        if (face.size() != 3)
            throw std::runtime_error("Can not parse non-triangular face.");

        for (const auto &f : face)
        {
            // OBJ have 1-based array indices.
            int vIdx = f.v - 1;
            int vnIdx = f.vn - 1;
            int vtIdx = f.vt - 1;
            glm::vec3 v = m_v[vIdx];
            glm::vec3 vn{0};
            if (vnIdx != -1)
                vn = m_vn[vnIdx];
            glm::vec2 vt{0};
            if (vtIdx != -1)
                vt = m_vt[vtIdx];

            Vertex vertex{
                {v[0], v[1], v[2]},
                {vn[0], vn[1], vn[2]},
                {0, 0, 0, 0}, // No colors
                {vt[0], vt[1]}};

            if (!vertexToIndex.count(vertex)) // New vertex
            {
                vertexToIndex[vertex] = vertices.size();
                indices.push_back(vertices.size());
                vertices.push_back(vertex);
            }
            else
            {
                indices.push_back(vertexToIndex[vertex]);
            }
        }
    }

    return std::make_pair(std::move(vertices), std::move(indices));
}

std::vector<glm::vec3> OBJ::getVertices() const
{
    auto pair = getVerticesIndices();
    std::vector<glm::vec3> result;
    for (const Vertex &v : pair.first)
        result.emplace_back(v.position[0], v.position[1], v.position[2]);

    return result;
}
