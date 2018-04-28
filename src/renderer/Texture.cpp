#include "Texture.hpp"
#include "Error.hpp"

Texture::Texture(const std::vector<GLubyte> &imageData, GLint internalFormat, GLsizei width, GLsizei height)
{
    GLint level = 0;
    GLint border = 0;
    GLenum type = GL_UNSIGNED_BYTE;
    glGenTextures(1, &m_id);
    glBindTexture(m_target, m_id);
    glTexImage2D(m_target, level, internalFormat, width, height, border, internalFormat, type, imageData.data());
    glGenerateMipmap(m_target);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Texture::setClampToEdge() const
{
    bind();
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::setRepeat() const
{
    bind();
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}