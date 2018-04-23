#include <array>
#include <glm/glm.hpp>
#include "loaders/OBJ.hpp"

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

    glm::vec3 getScale() const { return glm::vec3{m_max.x - m_min.x, m_max.y - m_min.y, m_max.z - m_min.z} * 0.5f; }

    AABB getRotated(const glm::vec3 &) const;
};
