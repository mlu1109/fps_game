#include <GL/glew.h>
#include <array>
#include <vector>


class TextureCubemap
{
    GLuint m_id;
    const GLenum m_target = GL_TEXTURE_CUBE_MAP;

  public:
    TextureCubemap();
    
    void loadSide(GLenum target, const std::vector<GLubyte> &imageData, GLint internalFormat, GLsizei width, GLsizei height);
   
    void bind() const { glBindTexture(m_target, m_id); }
    void destroy() { glDeleteTextures(1, &m_id); }
    void setRepeat() const;
    void setClampToEdge() const;
};