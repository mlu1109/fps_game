#include "TextureCubemap.hpp"

TextureCubemap::TextureCubemap()
{
    glGenTextures(1, &m_id);
    bind();
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}

void TextureCubemap::loadSide(GLenum sideTarget, const std::vector<GLubyte> &imageData, GLint internalFormat, GLsizei width, GLsizei height)
{
    bind();
    glTexImage2D(sideTarget, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, imageData.data());
}