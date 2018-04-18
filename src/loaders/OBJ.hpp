#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

/* 
 * Specification/Examples:
 * https://en.wikipedia.org/wiki/Wavefront_.obj_file
 * http://www.andrewnoske.com/wiki/OBJ_file_format
 */

struct FaceElement
{
    int v = -1;
    int vt = -1;
    int vn = -1;
};

struct OBJ
{
    std::vector<glm::vec3> v;
    std::vector<glm::vec2> vt;
    std::vector<glm::vec3> vn;
    std::vector<std::vector<FaceElement>> f;
    int cntV = 0;
    int cntVN = 0;
    int cntVT = 0;
    int cntF = 0;
    int cntFE = 0;
};

OBJ loadOBJ(const std::string &path);