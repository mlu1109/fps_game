#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Constants.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Cubemap.hpp"
#include "Shader.hpp"
#include "Error.hpp"

class Renderer
{
    GLenum m_primitive = GL_TRIANGLES;

  public:
    Renderer();

    void setPrimitive(GLenum mode) { m_primitive = mode; }
    void setShader(const Shader *);
    void setTexture(const Texture *, GLenum textureUnit = GL_TEXTURE0);
    void setCubemap(const Cubemap *, GLenum textureUnit = GL_TEXTURE0);

    void setUniformTime(float time) { glUniform1f(UNIFORM_TIME, time); }
    void setUniformModelWorld(const glm::mat4 &modelWorld);
    void setUniformWorldView(const glm::mat4 &worldView);
    void setUniformViewScreen(const glm::mat4 &viewScreen);
    void setUniformModelViewNormal(const glm::mat4 &modelViewNormal);
    void setUniformLightPosition(const glm::vec3 &lightPosition);
    void setUniformLightColor(const glm::vec3 &lightColor);
    void setUniformMaterialAmbient(const glm::vec3 &materialAmbient);
    void setUniformMaterialDiffuse(const glm::vec3 &materialDiffuse);
    void setUniformMaterialSpecular(const glm::vec3 &materialSpecular);
    void setUniformMaterialShine(float materialShine);
    void setUniformCameraPosition(const glm::vec3 &cameraPosition);
    void setUniformActivePointLights(int activePointLights);
    void setUniformPointLightPositions(const std::array<glm::vec3, 20> &pointLightPositions);
    void setUniformPointLightColors(const std::array<glm::vec3, 20> &pointLightColors);

    void enableWireframe() const { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    void disableWireframe() const { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    void enableBlend() const { glEnable(GL_BLEND); }
    void disableBlend() const { glDisable(GL_BLEND); }
    void enableDepthTest() const { glEnable(GL_DEPTH_TEST); }
    void disableDepthTest() const { glDisable(GL_DEPTH_TEST); }

    void clear();
    void render(const VertexArray *);
    void render(const VertexArray *, GLint idxFrom, GLsizei idxCount);
};
