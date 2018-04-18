#pragma once

#include <memory>
#include "renderer/VertexArray.hpp"
#include "renderer/Texture.hpp"
#include "renderer/Shader.hpp"

struct Mesh
{
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<VertexArray> vertexArray;
};