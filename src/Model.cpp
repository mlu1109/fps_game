#include "Model.hpp"
#include <iostream>

Model::Model(const VertexArray *vertexArray, const std::vector<Material> &materials, const std::vector<int> &indexMaterialId)
    : m_vertexArray(vertexArray), m_materials(materials), m_indexMaterialId(indexMaterialId)
{
}

void Model::render(Renderer &renderer, const glm::mat4 &worldView) const
{
    size_t processed = 0;
    while (processed != m_vertexArray->getIndexCount())
    {
        int curMaterialId = m_indexMaterialId[processed];
        size_t idxCount = 1;
        for (; processed + idxCount < m_indexMaterialId.size(); ++idxCount)
            if (m_indexMaterialId[processed + idxCount] != curMaterialId)
                break;

        renderer.setUniformModelWorld(m_modelWorld);
        renderer.setUniformModelViewNormal(glm::transpose(glm::inverse(worldView * m_modelWorld)));
        if (curMaterialId != -1)
        {
            const Material &m = m_materials[curMaterialId];
            renderer.setUniformMaterialAmbient(m.ambient);
            renderer.setUniformMaterialDiffuse(m.diffuse);
            renderer.setUniformMaterialSpecular(m.specular);
            renderer.setUniformMaterialShine(m.shine);
        }
        else
        {
            // Set default value
        }
        
        renderer.render(m_vertexArray, processed, idxCount);
        processed += idxCount;
    }
}

void Model::update(const Transform &t)
{
    m_modelWorld = t.getMatrix();
}