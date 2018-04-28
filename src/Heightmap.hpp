#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include "renderer/Drawable.hpp"
#include "renderer/TGA.hpp"
#include "renderer/Vertex.hpp"

class Heightmap : public Drawable
{
    int m_width;
    int m_height;
    int m_quadWidth;
    int m_quadHeight;

    std::string m_shader;
    std::string m_texture;
    std::string m_vertexArray;
    
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

    glm::mat4 m_modelView{};

  public:
    Heightmap(int width, int height, const std::vector<unsigned char> &data, int stride, const std::string &shader, const std::string &texture);
    Heightmap(const TGA &tga, const std::string &shader, const std::string &texture);
    // Drawable
    const std::string &getShader() const override { return m_shader; }
    const std::string &getTexture() const override { return m_texture; }
    const std::string &getVertexArray() const override { return m_vertexArray; }
    const glm::mat4 &getModelWorld() const override { return m_modelView; }
    // Are used for building VertexArray
    const std::vector<Vertex> &getVertices() const { return m_vertices; }
    const std::vector<GLuint> &getIndices() const { return m_indices; }
    
    float getY(float x, float z) const;
    float getY(const glm::vec3 &v) const { return getY(v.x, v.z); }

    void setVertexArray(const std::string &vertexArray) { m_vertexArray = vertexArray; }
};