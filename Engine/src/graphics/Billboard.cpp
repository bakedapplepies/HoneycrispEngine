#include "api/graphics/Billboard.h"
#include "api/ecs/ECSManager.h"
#include "api/managers/ShaderManager.h"
#include "api/components/DrawData.h"
#include "api/components/Transform.h"

HNCRSP_NAMESPACE_START

Billboard::Billboard(uint32_t width, uint32_t height)
{
    std::vector<glm::vec3> vertices = {
        glm::vec3(-1.0f,  1.0f,  0.0f) * glm::vec3(width, height, 0.0f),
        glm::vec3( 1.0f,  1.0f,  0.0f) * glm::vec3(width, height, 0.0f),
        glm::vec3( 1.0f, -1.0f,  0.0f) * glm::vec3(width, height, 0.0f),
        glm::vec3(-1.0f, -1.0f,  0.0f) * glm::vec3(width, height, 0.0f)
    };

    std::vector<glm::vec2> uvs = {
        glm::vec2(0.0f,  1.0f),
        glm::vec2(1.0f,  1.0f),
        glm::vec2(1.0f,  0.0f),
        glm::vec2(0.0f,  0.0f)  
    };

    std::vector<GLuint> indices = {
        0, 1, 3,
        1, 2, 3
    };

    m_mesh = Mesh(
        &vertices,
        &indices,
        nullptr,
        nullptr,
        &uvs,
        nullptr
    );
}

void Billboard::virt_AddDrawDataToRenderer(ECS::EntityUID entityUID, const Material& material) const
{
    m_mesh.virt_AddDrawDataToRenderer(entityUID, material);

    // Entity already has Transform (SceneRenderObj)
    g_ECSManager.GetComponent<Transform>(entityUID).lookAtCamera = true;
}

HNCRSP_NAMESPACE_END