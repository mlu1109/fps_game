#include "Texture.hpp"


Texture::Texture(const std::vector<GLubyte> &imageData, GLint internalFormat, GLsizei width, GLsizei height)
    : m_imageData{imageData}, m_internalFormat{internalFormat}, m_width{width}, m_height{height}
{
    glGenTextures(1, &m_id);
    glBindTexture(m_target, m_id);

    glGenerateMipmap(m_target);
    glTexImage2D(m_target, m_level, m_internalFormat, m_width, m_height, m_border, m_internalFormat, m_type, m_imageData.data());
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