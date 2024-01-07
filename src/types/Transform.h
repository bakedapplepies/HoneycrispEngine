#include "../pch/pch.h"


struct Transform
{
    glm::vec3 position;
    glm::vec3 eulerAngles;
    glm::vec3 scale;

    Transform() = default;
    explicit Transform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
        : position(position), eulerAngles(eulerAngles), scale(scale)
    {}
    Transform(const Transform& other);
    Transform& operator=(const Transform& other);
    Transform(Transform&& other) noexcept;
    Transform& operator=(Transform&& other) noexcept;
    ~Transform() = default;
};