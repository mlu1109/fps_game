#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mouse.hpp"
#include "renderer/Window.hpp"

void initCallbacks(Window *window, Mouse *mouse, Camera *camera);
