#include <glm/glm.hpp>
#include "loaders/OBJ.hpp"
#include "Transform.hpp"

#include "glm/print.hpp"

class AABB
{
    glm::vec3 m_min;
    glm::vec3 m_max;

  public:
    AABB() = default;
    AABB(const glm::vec3 min, glm::vec3 max) : m_min(min), m_max(max) {}
    AABB(const OBJ &);

    const glm::vec3 &getMin() const { return m_min; }
    const glm::vec3 &getMax() const { return m_max; }

    glm::vec3 getScale() const { return glm::vec3(m_max.x - m_min.x, m_max.y - m_min.y, m_max.z - m_min.z) * 0.5f; }
    glm::vec3 getCenter() const { return glm::vec3(m_max.x + m_min.x, m_max.y + m_min.y, m_max.z + m_min.z) * 0.5f; }
    Transform getTransform() const { std::cout << getCenter() << '\n'; return Transform{getCenter(), {0, 0, 0}, getScale()}; }
    AABB getTransformed(const glm::vec3 &T, const glm::vec3 &R, const glm::vec3 &S) const;
};
