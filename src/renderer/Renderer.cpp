#include "Error.hpp"
#include "Renderer.hpp"
#include "constants.hpp"

Renderer::Renderer(GLFWwindow *window, Camera *camera)
    : m_window{window}, m_camera{camera}
{
    glClearColor(0.2, 0.2, 0.5, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::setShader(const std::string &shader)
{
    if (m_boundShader == shader)
        return;

    m_boundShader = shader;
    const Shader &s = m_resourceManager.getShader(shader);
    s.bind();
    const glm::mat4 &worldView = m_camera->getWorldView();
    const glm::mat4 &viewScreen = m_camera->getViewScreen();
    glUniformMatrix4fv(UNIFORM_WORLD_TO_VIEW, 1, GL_FALSE, &worldView[0][0]);
    glUniformMatrix4fv(UNIFORM_VIEW_TO_SCREEN, 1, GL_FALSE, &viewScreen[0][0]);
}

void Renderer::pre()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderSkybox(const std::string &shader, const std::string &cubemap, const std::string &vertexArray)
{
    glDisable(GL_DEPTH_TEST);
    m_boundShader.clear();
    m_resourceManager.getShader(shader).bind();
    glm::mat4 worldView = m_camera->getWorldView();
    worldView[3] = glm::vec4{0, 0, 0, 1.0f};
    const glm::mat4 &viewScreen = m_camera->getViewScreen();
    glUniformMatrix4fv(UNIFORM_WORLD_TO_VIEW, 1, GL_FALSE, &worldView[0][0]);
    glUniformMatrix4fv(UNIFORM_VIEW_TO_SCREEN, 1, GL_FALSE, &viewScreen[0][0]);

    m_resourceManager.getCubemap(cubemap).bind();
    const VertexArray &va = m_resourceManager.getVertexArray(vertexArray);
    va.bind();
    GLsizei indexCount = va.getIndexCount();
    if (indexCount > 0)
        glDrawElements(m_primitive, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(m_primitive, 0, va.getVertexCount());
    glEnable(GL_DEPTH_TEST);
}

void Renderer::render(const std::string &shader, const std::string &texture, const std::string &vertexArray, const glm::mat4 &modelWorld)
{
    // Shader
    setShader(shader);
    // Texture
    if (texture.size())
    {
        const Texture &t = m_resourceManager.getTexture(texture);
        t.bind();
    }
    // VertexArray
    const VertexArray &v = m_resourceManager.getVertexArray(vertexArray);
    v.bind();
    glUniformMatrix4fv(UNIFORM_MODEL_TO_WORLD, 1, GL_FALSE, &modelWorld[0][0]);
    // Draw
    GLsizei indexCount = v.getIndexCount();
    if (indexCount > 0)
        glDrawElements(m_primitive, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(m_primitive, 0, v.getVertexCount());
}

void Renderer::render(const Drawable &object)
{
    render(object.getShader(), object.getTexture(), object.getVertexArray(), object.getModelWorld());
}

void Renderer::post()
{
    printError("Renderer::post");
    m_boundShader.clear();
    glfwSwapBuffers(m_window);
}