#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Constants.hpp"
#include "ResourceManager.hpp"

class Renderer
{
    GLenum m_primitive = GL_TRIANGLES;

  public:
    Renderer();

    void setPrimitive(GLenum mode) { m_primitive = mode; }
    void setShader(const Shader *);
    void setTexture(const Texture *, GLenum textureUnit = GL_TEXTURE0);
    void setCubemap(const Cubemap *, GLenum textureUnit = GL_TEXTURE0);

    //void setUniformColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { glUniform4f(UNIFORM_COLOR, r / 255.0, g / 255.0, b / 255.0, a / 255.0); }
    void setUniformTime(float time) { glUniform1f(UNIFORM_TIME, time); }
    void setUniformModelWorld(const glm::mat4 &modelWorld) { glUniformMatrix4fv(UNIFORM_MODEL_WORLD, 1, GL_FALSE, &modelWorld[0][0]); }
    void setUniformWorldView(const glm::mat4 &worldView) { glUniformMatrix4fv(UNIFORM_WORLD_VIEW, 1, GL_FALSE, &worldView[0][0]); }
    void setUniformViewScreen(const glm::mat4 &viewScreen) { glUniformMatrix4fv(UNIFORM_VIEW_SCREEN, 1, GL_FALSE, &viewScreen[0][0]); }

    void enableWireframe() const { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    void disableWireframe() const { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    void enableBlend() const { glEnable(GL_BLEND); }
    void disableBlend() const { glDisable(GL_BLEND); }

    void clear();
    void render(const VertexArray *);
};