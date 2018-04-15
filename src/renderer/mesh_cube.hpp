#pragma once

#include <GL/glew.h>
#include <vector>
#include "mesh.hpp"
#include "vertex.hpp"

/* Primarily used for testing */

namespace Meshes
{

Mesh getCubeIdx() // With indices
{
    const GLfloat n = 0.57735026919;
    const std::vector<Vertex> vertices{
        // Skipping colors and texCoord for now
        // front
        {{-0.5, -0.5, +0.5}, {-n, -n, +n}, {}, {}}, // 0
        {{+0.5, -0.5, +0.5}, {+n, -n, +n}, {}, {}}, // 1
        {{+0.5, +0.5, +0.5}, {+n, +n, +n}, {}, {}}, // 2
        {{-0.5, +0.5, +0.5}, {-n, +n, +n}, {}, {}}, // 3
        // back
        {{-0.5, -0.5, -0.5}, {-n, -n, -n}, {}, {}}, // 4
        {{+0.5, -0.5, -0.5}, {+n, -n, -n}, {}, {}}, // 5
        {{+0.5, +0.5, -0.5}, {+n, +n, -n}, {}, {}}, // 6
        {{-0.5, +0.5, -0.5}, {-n, +n, -n}, {}, {}}, // 7
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

    return {vertices, indices};
}

Mesh getCube() // Without indices
{
    const GLfloat n = 0.57735026919;
    const std::vector<Vertex> vertices{
        // front
        {{-0.5, -0.5, +0.5}, {-n, -n, +n}, {}, {}}, // 0
        {{+0.5, -0.5, +0.5}, {+n, -n, +n}, {}, {}}, // 1
        {{+0.5, +0.5, +0.5}, {+n, +n, +n}, {}, {}}, // 2
        {{+0.5, +0.5, +0.5}, {+n, +n, +n}, {}, {}}, // 2
        {{-0.5, +0.5, +0.5}, {-n, +n, +n}, {}, {}}, // 3
        {{-0.5, -0.5, +0.5}, {-n, -n, +n}, {}, {}}, // 0
        // top
        {{+0.5, -0.5, +0.5}, {+n, -n, +n}, {}, {}}, // 1
        {{+0.5, -0.5, -0.5}, {+n, -n, -n}, {}, {}}, // 5
        {{+0.5, +0.5, -0.5}, {+n, +n, -n}, {}, {}}, // 6
        {{+0.5, +0.5, -0.5}, {+n, +n, -n}, {}, {}}, // 6
        {{+0.5, +0.5, +0.5}, {+n, +n, +n}, {}, {}}, // 2
        {{+0.5, -0.5, +0.5}, {+n, -n, +n}, {}, {}}, // 1
        // back
        {{-0.5, +0.5, -0.5}, {-n, +n, -n}, {}, {}}, // 7
        {{+0.5, +0.5, -0.5}, {+n, +n, -n}, {}, {}}, // 6
        {{+0.5, -0.5, -0.5}, {+n, -n, -n}, {}, {}}, // 5
        {{+0.5, -0.5, -0.5}, {+n, -n, -n}, {}, {}}, // 5
        {{-0.5, -0.5, -0.5}, {-n, -n, -n}, {}, {}}, // 4
        {{-0.5, +0.5, -0.5}, {-n, +n, -n}, {}, {}}, // 7
        // bottom
        {{-0.5, -0.5, -0.5}, {-n, -n, -n}, {}, {}}, // 4
        {{-0.5, -0.5, +0.5}, {-n, -n, +n}, {}, {}}, // 0
        {{-0.5, +0.5, +0.5}, {-n, +n, +n}, {}, {}}, // 3
        {{-0.5, +0.5, +0.5}, {-n, +n, +n}, {}, {}}, // 3
        {{-0.5, +0.5, -0.5}, {-n, +n, -n}, {}, {}}, // 7
        {{-0.5, -0.5, -0.5}, {-n, -n, -n}, {}, {}}, // 4
        // left
        {{-0.5, -0.5, -0.5}, {-n, -n, -n}, {}, {}}, // 4
        {{+0.5, -0.5, -0.5}, {+n, -n, -n}, {}, {}}, // 5
        {{+0.5, -0.5, +0.5}, {+n, -n, +n}, {}, {}}, // 1
        {{+0.5, -0.5, +0.5}, {+n, -n, +n}, {}, {}}, // 1
        {{-0.5, -0.5, +0.5}, {-n, -n, +n}, {}, {}}, // 0
        {{-0.5, -0.5, -0.5}, {-n, -n, -n}, {}, {}}, // 4
        // right
        {{-0.5, +0.5, +0.5}, {-n, +n, +n}, {}, {}}, // 3
        {{+0.5, +0.5, +0.5}, {+n, +n, +n}, {}, {}}, // 2
        {{+0.5, +0.5, -0.5}, {+n, +n, -n}, {}, {}}, // 6
        {{+0.5, +0.5, -0.5}, {+n, +n, -n}, {}, {}}, // 6
        {{-0.5, +0.5, -0.5}, {-n, +n, -n}, {}, {}}, // 7
        {{-0.5, +0.5, +0.5}, {-n, +n, +n}, {}, {}}, // 3
    };
    
    return {vertices, std::vector<GLuint>{}};
}

} // namespace Meshes