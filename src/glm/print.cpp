#include "print.hpp"
#include <iomanip>

std::ostream &operator<<(std::ostream &os, const glm::vec3 &v)
{
    os << "["  << std::setw(4) << v.x << std::setw(4) << v.y << std::setw(4) << v.z << std::setw(4) << "]";
    return os;
}

std::ostream &operator<<(std::ostream &os, const glm::vec4 &v)
{
    os << "["  << std::setw(4) << v.x << std::setw(4) << v.y << std::setw(4) << v.z << std::setw(4) << v.w  << std::setw(4) << "]";
    return os;
}

std::ostream &operator<<(std::ostream &os, const glm::mat4 &m)
{
    os << m[0] << '\n'
       << m[1] << '\n'
       << m[2] << '\n'
       << m[3] << '\n';
    return os;
}
