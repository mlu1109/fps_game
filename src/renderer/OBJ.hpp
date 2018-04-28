#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Vertex.hpp"

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

class OBJ
{
    std::vector<glm::vec3> m_v;
    std::vector<glm::vec2> m_vt;
    std::vector<glm::vec3> m_vn;
    std::vector<std::vector<FaceElement>> m_f;

    void parseV(const std::string &args);
    void parseVT(const std::string &args);
    void parseVN(const std::string &args);
    void parseF(const std::string &args);

  public:
    OBJ(const std::string &path);
    
    std::pair<std::vector<Vertex>, std::vector<GLuint>> getVerticesIndices() const;
    std::vector<glm::vec3> getVertices() const;
};
