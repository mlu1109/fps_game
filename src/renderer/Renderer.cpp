#include "Error.hpp"
#include "Renderer.hpp"

Renderer::Renderer()
{
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::setShader(const Shader *shader)
{
    shader->bind();
    printError("Renderer::setShader");
}

void Renderer::setCubemap(const Cubemap *cm, GLenum textureUnit)
{
    if (textureUnit != GL_TEXTURE0)
        glActiveTexture(textureUnit);

    cm->bind();

    if (textureUnit != GL_TEXTURE0)
        glActiveTexture(GL_TEXTURE0);

    printError("Renderer::setCubemap");
}

void Renderer::setTexture(const Texture *t, GLenum textureUnit)
{
    if (textureUnit != GL_TEXTURE0)
        glActiveTexture(textureUnit);

    t->bind();

    if (textureUnit != GL_TEXTURE0)
        glActiveTexture(GL_TEXTURE0);

    printError("Renderer::setTexture");
}

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    printError("Renderer::clear");
}

void Renderer::render(const VertexArray *va)
{
    va->bind();
    GLsizei indexCount = va->getIndexCount();
    if (indexCount > 0)
        glDrawElements(m_primitive, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(m_primitive, 0, va->getVertexCount());
    printError("Renderer::render");
}

void Renderer::render(const VertexArray *va, GLint idxFrom, GLsizei idxCount)
{
    va->bind();
    //glDrawElements(m_primitive, idxCount, GL_UNSIGNED_INT, (void *)(idxFrom * sizeof(GLuint)));
    glDrawElements(m_primitive, va->getIndexCount(), GL_UNSIGNED_INT, 0);
    printError("Renderer::render idxFrom: " + std::to_string(idxFrom) + " " + "idxCount: " + std::to_string(idxCount));
}

void Renderer::setUniformModelWorld(const glm::mat4 &modelWorld)
{
    glUniformMatrix4fv(UNIFORM_MODEL_WORLD, 1, GL_FALSE, &modelWorld[0][0]);
    printError("Renderer::setUniformModelWorld");
}

void Renderer::setUniformWorldView(const glm::mat4 &worldView)
{
    glUniformMatrix4fv(UNIFORM_WORLD_VIEW, 1, GL_FALSE, &worldView[0][0]);
    printError("Renderer::setUniformWorldView");
}

void Renderer::setUniformViewScreen(const glm::mat4 &viewScreen)
{
    glUniformMatrix4fv(UNIFORM_VIEW_SCREEN, 1, GL_FALSE, &viewScreen[0][0]);
    printError("Renderer::setUniformViewScreen");
}

void Renderer::setUniformModelViewNormal(const glm::mat4 &modelViewNormal)
{
    glUniformMatrix4fv(UNIFORM_MODEL_VIEW_NORMAL, 1, GL_FALSE, &modelViewNormal[0][0]);
    printError("Renderer::setUniformModelViewNormal");
}

void Renderer::setUniformLightPosition(const glm::vec3 &lightPosition)
{
    glUniform3fv(UNIFORM_LIGHT_POSITION, 1, &lightPosition[0]);
    printError("Renderer::setUniformLightPosition");
}

void Renderer::setUniformLightColor(const glm::vec3 &lightColor)
{
    glUniform3fv(UNIFORM_LIGHT_COLOR, 1, &lightColor[0]);
    printError("Renderer::setUniformLightColor");
}

void Renderer::setUniformMaterialAmbient(const glm::vec3 &materialAmbient)
{
    glUniform3fv(UNIFORM_MATERIAL_AMBIENT, 1, &materialAmbient[0]);
    printError("Renderer::setUniformMaterialAmbient");
}

void Renderer::setUniformMaterialDiffuse(const glm::vec3 &materialDiffuse)
{
    glUniform3fv(UNIFORM_MATERIAL_DIFFUSE, 1, &materialDiffuse[0]);
    printError("Renderer::setUniformMaterialDiffuse");
}

void Renderer::setUniformMaterialSpecular(const glm::vec3 &materialSpecular)
{
    glUniform3fv(UNIFORM_MATERIAL_SPECULAR, 1, &materialSpecular[0]);
    printError("Renderer::setUniformMaterialSpecular");
}

void Renderer::setUniformMaterialShine(float materialShine)
{
    glUniform1f(UNIFORM_MATERIAL_SHINE, materialShine);
    printError("Renderer::setUniformMaterialShine");
}

void Renderer::setUniformCameraPosition(const glm::vec3 &cameraPosition)
{
    glUniform3fv(UNIFORM_CAMERA_POSITION, 1, &cameraPosition[0]);
    printError("Renderer::setUniformCameraPosition");
}
