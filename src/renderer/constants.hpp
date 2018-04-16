#pragma once

#include <GL/glew.h>

// Shader Attribute Locations
const GLuint ATTRIB_POSITION = 0;
const GLuint ATTRIB_NORMAL = 1;
const GLuint ATTRIB_COLOR = 2;
const GLuint ATTRIB_TEX_COORD = 3;

// Shader Uniform Locations
const GLuint UNIFORM_MODEL_TO_WORLD = 0;
const GLuint UNIFORM_WORLD_TO_VIEW = 1;
const GLuint UNIFORM_VIEW_TO_SCREEN = 2;
const GLuint UNIFORM_NORMAL = 3;
const GLuint UNIFORM_TEXTURE_UNIT = 4;