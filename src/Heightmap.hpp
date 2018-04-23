#include <glm/gtx/transform.hpp>
#include "Mesh.hpp"
#include "renderer/Vertex.hpp"
#include "renderer/Drawable.hpp"


class HeightMap : public Drawable
{
    Mesh m_mesh;
    int m_width;
    int m_height;
    glm::mat4 m_modelView{};

  public:
    HeightMap(int width, int height, const std::vector<unsigned char> &data, int stride, const std::shared_ptr<Shader> &shader, const std::shared_ptr<Texture> &texture);
    // Drawable
    const std::shared_ptr<Shader> &getShader() const override { return m_mesh.shader; }
    const std::shared_ptr<Texture> &getTexture() const override { return m_mesh.texture; }
    const std::shared_ptr<VertexArray> &getVertexArray() const override { return m_mesh.vertexArray; }
    const glm::mat4 &getModelWorld() const override { return m_modelView; }
};
