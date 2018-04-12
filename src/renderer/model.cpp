#include <GL/glew.h>
#include "constants.hpp" // UNIFORM_...
#include "model.hpp"

Model::Model(const std::vector<VertexArray *> &meshes, const glm::mat4 &translate, const glm::mat4 &scale, const glm::mat4 &rotate)
    : m_meshes{meshes}, m_translate{translate}, m_rotate{rotate}, m_scale{scale}
{
}

void Model::draw() const
{
    /* NOTE: Assumes that program and uniforms have already been set */
    for (const auto &mesh : m_meshes) 
    {
        mesh->bind();
        GLsizei indexCount = mesh->getIndexCount();
        if (indexCount > 0)
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, mesh->getVertexCount());
    }
}