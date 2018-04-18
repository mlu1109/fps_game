#pragma once

#include <GL/glew.h>
#include <memory>
#include <vector>
#include "loaders/OBJ.hpp"
#include "loaders/TGA.hpp"
#include "renderer/Texture.hpp"
#include "renderer/VertexArray.hpp"

/*
 * A bunch of helper functions that have not found a better home
 */

std::shared_ptr<VertexArray> newVertexArrayFromOBJ(const std::string &objPath);
std::shared_ptr<Texture> newTextureFromTGA(const std::string &tgaPath);