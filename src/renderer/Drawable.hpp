#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

class Drawable
{
  public:
    virtual const std::shared_ptr<Shader> &getShader() const = 0;
    virtual const std::shared_ptr<Texture> &getTexture() const = 0;
    virtual const std::shared_ptr<VertexArray> &getVertexArray() const = 0;
    virtual const glm::mat4 &getModelWorld() const = 0;
}; 