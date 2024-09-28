#include "Billboard.h"
#include "src/ecs/ECSManager.h"
#include "src/managers/ShaderManager.h"
#include "src/components/DrawData.h"


HNCRSP_NAMESPACE_START

Billboard::Billboard(uint32_t width, uint32_t height, const Texture2D* texture)
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

    m_VAO = VertexArray(
        &vertices,
        &indices,
        nullptr,
        nullptr,
        &uvs
    );

    m_texture = texture;
}

void Billboard::virt_AddDrawDataToRenderer(ECS::EntityUID entityUID) const
{
    // Entity already has Transform (SceneRenderObj)
    g_ECSManager.GetComponent<Transform>(entityUID).lookAtCamera = true;

    DrawData drawData;
    drawData.VAO_id = m_VAO.GetID();
    drawData.meta_data.emplace_back(0, m_VAO.GetIndicesLen(), 0);
    drawData.materials.push_back(std::make_shared<Material>(g_ShaderManager.albedoShader));

    drawData.materials[0]->SetAlbedoMap(m_texture);
    g_ECSManager.AddComponent<DrawData>(entityUID, drawData);
}

HNCRSP_NAMESPACE_END