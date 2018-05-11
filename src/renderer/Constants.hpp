#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

const GLuint ATTRIB_POSITION = 0;
const GLuint ATTRIB_NORMAL = 1;
const GLuint ATTRIB_TEX_COORD = 2;

const GLuint UNIFORM_MODEL_WORLD = 0;
const GLuint UNIFORM_WORLD_VIEW = 1;
const GLuint UNIFORM_VIEW_SCREEN = 2;
const GLuint UNIFORM_MODEL_VIEW_NORMAL = 3;

const GLuint UNIFORM_TIME = 7;

const double M_HALF_PI = M_PI/2;