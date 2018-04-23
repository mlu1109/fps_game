#include "Renderer.hpp"
#include "constants.hpp"

Renderer::Renderer(GLFWwindow *window, Camera *camera)
    : m_window{window}, m_camera{camera}
{
    glClearColor(0.2, 0.2, 0.5, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::setShader(const std::shared_ptr<Shader> &shader)
{
    if (m_boundShader == shader)
        return;

    shader->bind();
    m_boundShader = shader;
    const glm::mat4 &worldView = m_camera->getWorldView();
    const glm::mat4 &viewScreen = m_camera->getViewScreen();
    glUniformMatrix4fv(UNIFORM_WORLD_TO_VIEW, 1, GL_FALSE, &worldView[0][0]);
    glUniformMatrix4fv(UNIFORM_VIEW_TO_SCREEN, 1, GL_FALSE, &viewScreen[0][0]);
}

void Renderer::pre()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const std::shared_ptr<Texture> &texture, const glm::mat4 &modelWorld)
{
    setShader(shader);
    vertexArray->bind();
    glUniformMatrix4fv(UNIFORM_MODEL_TO_WORLD, 1, GL_FALSE, &modelWorld[0][0]);

    if (texture)
        texture->bind();
    else if (m_textureMissing)
        m_textureMissing->bind();

    GLsizei indexCount = vertexArray->getIndexCount();
    if (indexCount > 0)
        glDrawElements(m_primitive, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(m_primitive, 0, vertexArray->getVertexCount());
}

void Renderer::render(const Drawable &object)
{
    render(object.getShader(), object.getVertexArray(), object.getTexture(), object.getModelWorld());
}

void Renderer::post()
{
    m_boundShader = nullptr;
    glfwSwapBuffers(m_window);
}