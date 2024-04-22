#include "Renderer.h"
#include "src/ecs/ECSManager.h"
#include "src/components/MeshData.h"


HNCRSP_NAMESPACE_START

Renderer::Renderer()
{
    HNCRSP_LOG_INFO("HEH");
}

void Renderer::StartUp()
{
}

void Renderer::Render() const
{
    GLuint shaderID = 0;
    if (auto cubemap = m_weak_currentCubemap.lock())
    {
        cubemap->Draw();
    }

    const Texture2D* albedoMap = nullptr;
    const Texture2D* roughnessMap = nullptr;
    const Texture2D* aoMap = nullptr;
    const Texture2D* normalMap = nullptr;
    const Texture2D* specularMap = nullptr;
    for (const EntityUID& uid : entityUIDs)
    {
        MeshData& meshData = g_ECSManager->GetComponent<MeshData>(uid);
        Material* material = meshData.material.get();
        Shader* shader = material->getShader().get();

        Transform& transform = g_ECSManager->GetComponent<Transform>(uid);

        // HNCRSP_LOG_INFO(uid);
        if (shaderID != shader->getID())
        {
            shader->Use();
            shaderID = shader->getID();
        }

        albedoMap = material->getAlbedoMap();
        roughnessMap = material->getRoughnessMap();
        aoMap = material->getAoMap();
        normalMap = material->getNormalMap();
        specularMap = material->getSpecularMap();

        if (albedoMap) albedoMap->Bind();
        // else shader->setIntUnf("u_material.albedo", 0);
        if (roughnessMap) roughnessMap->Bind();
        if (aoMap) aoMap->Bind();
        if (normalMap) albedoMap->Bind();
        if (specularMap) specularMap->Bind();

        glm::mat4 modelMatrix = GetModelMatrix(transform);
        shader->setMat3Unf("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix))));
        shader->setMat4Unf("u_model", modelMatrix);
        GLCall(
            glDrawElementsBaseVertex(GL_TRIANGLES, meshData.num_vertices, GL_UNSIGNED_INT, 0x0, 0)
        );

        if (albedoMap) albedoMap->Unbind();
        if (roughnessMap) roughnessMap->Unbind();
        if (aoMap) aoMap->Unbind();
        if (normalMap) albedoMap->Unbind();
        if (specularMap) specularMap->Unbind();
    }
}

// TODO: quaternions
glm::mat4 Renderer::GetModelMatrix(Transform& transform) const 
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, transform.position);
    
    glm::quat quaternion = glm::quat(transform.eulerAngles);
    glm::mat4 rotationMatrix = glm::toMat4(quaternion);
    // glm::rotate()
    modelMatrix *= rotationMatrix;
    // modelMatrix *= quaternion;

    modelMatrix = glm::scale(modelMatrix, transform.scale);

    return modelMatrix;
}

void Renderer::SwitchCubemap(std::weak_ptr<Cubemap> weak_cubemap)
{
    m_weak_currentCubemap = weak_cubemap;
}

void Renderer::AddEntityUID(EntityUID entityUID)
{
    GLuint shaderID = g_ECSManager->GetComponent<MeshData>(entityUID).material->getShader()->getID();
    binary_insert_shader_comparator(entityUIDs, m_shaderIDs_Order, entityUID, shaderID);
}

HNCRSP_NAMESPACE_END