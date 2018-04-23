#pragma once

#include <GL/glew.h>
#include <memory>
#include <utility>
#include <vector>
#include "Heightmap.hpp"
#include "loaders/OBJ.hpp"
#include "loaders/TGA.hpp"
#include "renderer/Texture.hpp"
#include "renderer/VertexArray.hpp"


// Vertex/Indices
std::pair<std::vector<Vertex>, std::vector<GLuint>> getVertexDataFromOBJ(const OBJ &);

// VertexArray
std::shared_ptr<VertexArray> newVertexArrayFromOBJ(const std::string &objPath);
std::shared_ptr<VertexArray> newVertexArrayFromOBJ(const OBJ &);

// Texture
std::shared_ptr<Texture> newTextureFromTGA(const std::string &fileName);

// HeightMap
HeightMap newHeightMapFromTGA(const std::string &tgaPath, const std::shared_ptr<Shader> &shader, const std::shared_ptr<Texture> &texture);
