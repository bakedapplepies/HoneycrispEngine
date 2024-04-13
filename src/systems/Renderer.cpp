#include "Renderer.h"
#include "src/ecs/ECSManager.h"
#include "src/components/MeshData.h"


HNCRSP_NAMESPACE_START

void Renderer::StartUp()
{
}

void Renderer::Render() const
{
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
        // HNCRSP_LOG_INFO(uid);
        MeshData& meshData = g_ECSManager->GetComponent<MeshData>(uid);
        Material* material = meshData.material.get();
        Shader* shader = material->getShader().get();

        Transform& transform = g_ECSManager->GetComponent<Transform>(uid);

        GLCall(glBindVertexArray(meshData.VAO_id));
        shader->Use();

        albedoMap = material->getAlbedoMap();
        roughnessMap = material->getRoughnessMap();
        aoMap = material->getAoMap();
        normalMap = material->getNormalMap();
        specularMap = material->getSpecularMap();

        if (albedoMap) albedoMap->Bind();
        if (roughnessMap)
            roughnessMap->Bind();
        if (aoMap) aoMap->Bind();
        if (normalMap) albedoMap->Bind();
        if (specularMap) specularMap->Bind();

        glm::mat4 modelMatrix = GetModelMatrix(transform);
        shader->setMat3Unf("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix))));
        shader->setMat4Unf("u_model", modelMatrix);
        GLCall(glDrawElements(GL_TRIANGLES, meshData.num_vertices, GL_UNSIGNED_INT, nullptr));

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

HNCRSP_NAMESPACE_END