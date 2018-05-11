#include "Error.hpp"
#include "Renderer.hpp"

Renderer::Renderer()
{
    glClearColor(0.2, 0.2, 0.5, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::setShader(const Shader *shader)
{
    shader->bind();
}

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const VertexArray *va)
{
    va->bind();
    GLsizei indexCount = va->getIndexCount();
    if (indexCount > 0)
        glDrawElements(m_primitive, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(m_primitive, 0, va->getVertexCount());
}