#pragma once

#include <GL/glew.h>
#include <vector>


class Texture
{
    GLuint m_id;
    std::vector<GLubyte> m_imageData; // Not used

    GLint m_internalFormat;
    GLsizei m_width;
    GLsizei m_height;
    GLenum m_target = GL_TEXTURE_2D;
    GLint m_level = 0;
    GLint m_border = 0;
    GLenum m_type = GL_UNSIGNED_BYTE;
    // glTexParameteri


  public:
    Texture(const std::vector<GLubyte> &imageData, GLint internalFormat, GLsizei width, GLsizei height);
    Texture(const Texture &) = delete;
    Texture(Texture &&) = delete;
    ~Texture() { glDeleteTextures(1, &m_id); }

    void bind() const { glBindTexture(m_target, m_id); }
    void setRepeat() const;
    void setClampToEdge() const;
};
