#include "Renderer.hpp"
#include "constants.hpp"

Renderer::Renderer(GLFWwindow *window, Camera *camera)
    : m_window{window}, m_camera{camera}
{
    glClearColor(0.2, 0.2, 0.5, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::pre()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 worldView = m_camera->getWorldView();
    glUniformMatrix4fv(UNIFORM_WORLD_TO_VIEW, 1, GL_FALSE, &worldView[0][0]);
    glm::mat4 viewScreen = m_camera->getViewScreen();
    glUniformMatrix4fv(UNIFORM_VIEW_TO_SCREEN, 1, GL_FALSE, &viewScreen[0][0]);
}

void Renderer::render(const Drawable &object)
{
    const auto& shader = object.getShader();
    const auto& va = object.getVertexArray();
    const auto& tex = object.getTexture();
    const auto& modelWorld = object.getModelWorld();

    shader->bind();
    va->bind();
    if (tex)
        tex->bind();
    glUniformMatrix4fv(UNIFORM_MODEL_TO_WORLD, 1, GL_FALSE, &modelWorld[0][0]);

    GLsizei indexCount = va->getIndexCount();
    if (indexCount > 0)
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, va->getVertexCount());
}

void Renderer::post()
{
    glfwSwapBuffers(m_window);
}