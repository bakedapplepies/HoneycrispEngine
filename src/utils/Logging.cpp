#include "Logging.h"


std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec2)
{
    std::cout << glm::to_string(vec2);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec3)
{
    std::cout << glm::to_string(vec3);
    return stream;
}

HNCRSP_NAMESPACE_START

namespace Log
{
    // namespace
    // {
    //     bool beganLine = true;
    // }
}

HNCRSP_NAMESPACE_END