#pragma once

#include <GL/glew.h>
#include <vector>


class Texture
{
    GLuint m_id;
    const GLenum m_target = GL_TEXTURE_2D;

  public:
    Texture(const std::vector<GLubyte> &imageData, GLint internalFormat, GLsizei width, GLsizei height);
    
    void bind() const { glBindTexture(m_target, m_id); }
    void destroy() { glDeleteTextures(1, &m_id); }
    void setRepeat() const;
    void setClampToEdge() const;
};
