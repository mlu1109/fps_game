#pragma once

#include <GL/glew.h>
#include <vector>
#include "../vertex.hpp"

/* Primarily used for testing */

namespace Meshes
{

namespace CubeIdx // Cube using indices
{

const GLfloat n = 0.57735026919;

const std::vector<Vertex> vertices{
    // Skipping colors and texCoord for now
    // front
    {{-0.5, -0.5, +0.5}, {-n, -n, +n}, {}, {}},
    {{+0.5, -0.5, +0.5}, {+n, -n, +n}, {}, {}},
    {{+0.5, +0.5, +0.5}, {+n, +n, +n}, {}, {}},
    {{-0.5, +0.5, +0.5}, {-n, +n, +n}, {}, {}},
    // back
    {{-0.5, -0.5, -0.5}, {-n, -n, -n}, {}, {}},
    {{+0.5, -0.5, -0.5}, {+n, -n, -n}, {}, {}},
    {{+0.5, +0.5, -0.5}, {+n, +n, -n}, {}, {}},
    {{-0.5, +0.5, -0.5}, {-n, +n, -n}, {}, {}},
};

const std::vector<GLuint> indices{
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 0, 3,
    3, 7, 4,
    // left
    4, 5, 1,
    1, 0, 4,
    // right
    3, 2, 6,
    6, 7, 3};
};
// TODO: Cube not using indices
}