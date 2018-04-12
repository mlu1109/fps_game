#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer/renderer.hpp"
#include "mouse.hpp"

void initCallbacks(GLFWwindow *window, Mouse *mouse, Camera *camera);
