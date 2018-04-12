#include "constants.hpp"
#include "renderer.hpp"

Renderer::Renderer(GLFWwindow *window, Camera *camera, GLuint program)
    : m_window{window}, m_camera{camera}, m_program{program}
{
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::render(const std::vector<Model> &models)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_program); // TODO: Temporary, fix this.
    
    glm::mat4 worldView = m_camera->getWorldToView();
    glUniformMatrix4fv(UNIFORM_WORLD_TO_VIEW, 1, GL_FALSE, &worldView[0][0]);
    glm::mat4 viewScreen = m_camera->getViewToScreen();
    glUniformMatrix4fv(UNIFORM_VIEW_TO_SCREEN, 1, GL_FALSE, &viewScreen[0][0]);
    for (const auto &model : models)
    {
        glm::mat4 modelWorld = model.getModelToWorld();
        glUniformMatrix4fv(UNIFORM_MODEL_TO_WORLD, 1, GL_FALSE, &modelWorld[0][0]);
        model.draw();
    }

    glfwSwapBuffers(m_window);
}