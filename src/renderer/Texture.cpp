#include "Texture.hpp"

Texture::Texture(const std::vector<GLubyte> &imageData, GLint internalFormat, GLsizei width, GLsizei height)
    : m_imageData{imageData}, m_internalFormat{internalFormat}, m_width{width}, m_height{height}
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, m_level, m_internalFormat, m_width, m_height, m_border, m_internalFormat, m_type, m_imageData.data());
}
