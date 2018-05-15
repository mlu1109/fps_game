#include "Ray.hpp"

Ray::Ray(int screenX, int screenY, int screenWidth, int screenHeight, const Camera &camera)
{
    // Reference: http://antongerdelan.net/opengl/raycasting.html
    
    glm::vec4 rayDeviceCoords(
        (2.0f * screenX) / screenWidth - 1.0f,
        1.0f - (2.0f * screenY) / screenHeight,
        -1.0f, // OpenGL forward
        1.0f);

    glm::vec4 rayViewCoords = glm::inverse(camera.getViewScreen()) * rayDeviceCoords;
    rayViewCoords.w = 0.0f; // Do not translate, we are looking for the direction
    glm::vec3 rayWorldCoords = glm::inverse(camera.getWorldView()) * rayViewCoords;
    origin = camera.getPosition();
    direction = glm::normalize(rayWorldCoords);
}