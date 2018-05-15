#pragma once

#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "renderer/VertexArray.hpp"
#include "renderer/Renderer.hpp"

class Skybox
{
    const Shader *m_shader;
    const Cubemap *m_cubemap;
    const VertexArray *m_vertexArray;

    // Global light source, e.g. sun
    glm::vec3 m_lightPosition;
    glm::vec3 m_lightColor;

  public:
    Skybox(const Shader *s, const Cubemap *c, const VertexArray *va)
    : m_shader(s), m_cubemap(c), m_vertexArray(va) {}

    void setLightPosition(const glm::vec3 lp) { m_lightPosition = lp; }
    void setLightColor(const glm::vec3 lc) { m_lightColor = lc; }
    const glm::vec3 &getLightPosition() const { return m_lightPosition; }
    const glm::vec3 &getLightColor() const { return m_lightColor; }
    void render(Renderer&, glm::mat4 worldView, const glm::mat4 &viewScreen) const;

};
