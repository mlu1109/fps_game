#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

class Drawable
{
  public:
    virtual const std::string &getShader() const = 0;
    virtual const std::string &getTexture() const = 0;
    virtual const std::string &getVertexArray() const = 0;
    virtual const glm::mat4 &getModelWorld() const = 0;
}; 