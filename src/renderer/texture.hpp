#include <GL/glew.h>
#include <vector>

struct Texture
{
    std::vector<GLubyte> imageData;
    GLuint bitsPerPixel;
    GLuint width;
    GLuint height;
    GLuint textureId;
    GLuint type;
};
