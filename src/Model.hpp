#include "renderer/OBJ.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/VertexArray.hpp"
#include "Transform.hpp"

class Model
{
    const VertexArray *m_vertexArray;
    const std::vector<Material> m_materials;
    const std::vector<int> m_indexMaterialId;
    glm::mat4 m_modelWorld;

  public:
    Model(const VertexArray *, const std::vector<Material> &, const std::vector<int> &indexMaterialId);
    const VertexArray *getVertexArray() const { return m_vertexArray; }
    const std::vector<Material> &getMaterials() const { return m_materials; }
    const std::vector<int> m_IndexMaterialId() const { return m_indexMaterialId; }

    void render(Renderer&, const glm::mat4 &worldView) const;
    void update(const Transform &);
};
