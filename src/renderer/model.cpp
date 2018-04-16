#include <GL/glew.h>
#include "constants.hpp" // UNIFORM_...
#include "model.hpp"

Model::Model(const glm::mat4 &translate, const glm::mat4 &scale, const glm::mat4 &rotate)
    : m_translate{translate}, m_rotate{rotate}, m_scale{scale}
{
}

void Model::draw() const
{
    /* NOTE: Assumes that program and uniforms have already been set */
    for (const auto &mesh : m_meshes) 
    {
        // Bindings
        const VertexArray *va = mesh.getVertexArray();
        const Texture *tex = mesh.getTexture();
        va->bind();
        tex->bind();
        
        // Uniforms
        glUniformMatrix4fv(UNIFORM_MODEL_TO_WORLD, 1, GL_FALSE, &mesh.getMeshWorld()[0][0]);

        // Draw
        GLsizei indexCount = va->getIndexCount();
        if (indexCount > 0)
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, va->getVertexCount());
    }
}

void Model::updateModelWorld()
{
    m_modelWorld = m_translate * m_rotate * m_scale;
    for (auto &mesh : m_meshes)
        mesh.updateMeshWorld(m_modelWorld);
}