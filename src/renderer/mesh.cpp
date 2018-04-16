#include <exception>
#include "mesh.hpp"

Mesh::Mesh(VertexArray *va, Texture *tex, glm::mat4 translate, glm::mat4 rotate, glm::mat4 scale)
    : m_vertexArray{va}, m_texture{tex}, m_translate{translate}, m_rotate{rotate}, m_scale{scale}
{
    if (va == nullptr)
        throw std::invalid_argument("VertexArray* can not be nullptr");
    
    m_meshModel = m_translate * m_rotate * m_scale;
}
