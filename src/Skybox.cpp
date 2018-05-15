#include "Skybox.hpp"

void Skybox::render(Renderer& r, glm::mat4 worldView, const glm::mat4 &viewScreen) const
{
    r.setShader(m_shader);
    r.setCubemap(m_cubemap);
    r.disableDepthTest();
    worldView[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Clear translation
    r.setUniformWorldView(worldView);
    r.setUniformViewScreen(viewScreen);
    r.render(m_vertexArray);
    r.enableDepthTest();
}
