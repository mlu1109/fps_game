#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer/Renderer.hpp"
#include "Mouse.hpp"

void initCallbacks(GLFWwindow *window, Mouse *mouse, Camera *camera);
