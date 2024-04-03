#include "Renderer.h"
#include "src/ecs/ECSManager.h"
#include "src/components/MeshData.h"


HNCRSP_NAMESPACE_START

void Renderer::StartUp()
{
    if (m_startedUp == true)
    {
        HNCRSP_TERMINATE("Renderer already started up.");
    }
    m_startedUp = true;
    m_basicShader = std::make_shared<Shader>(
        FileSystem::Path("resources/shaders/DefaultVertex.glsl"),
        FileSystem::Path("resources/shaders/DefaultFragment.glsl")
    );
}

void Renderer::Render() const
{
    const Texture2D* albedoMap;
    const Texture2D* roughnessMap;
    const Texture2D* aoMap;
    const Texture2D* normalMap;
    const Texture2D* specularMap;
    for (const EntityUID& uid : entityUIDs)
    {
        MeshData& meshData = g_ECSManager->GetComponent<MeshData>(uid);
        std::shared_ptr<Material> material = meshData.material;
        std::shared_ptr<Shader> shader = material->getShader();

        Transform& transform = g_ECSManager->GetComponent<Transform>(uid);

        GLCall(glBindVertexArray(meshData.VAO_id));
        meshData.material->getShader()->Use();

        albedoMap = material->getAlbedoMap();
        roughnessMap = material->getRoughnessMap();
        aoMap = material->getAoMap();
        normalMap = material->getNormalMap();
        specularMap = material->getSpecularMap();

        if (albedoMap) albedoMap->Bind();
        if (roughnessMap) roughnessMap->Bind();
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
    modelMatrix *= rotationMatrix;

    modelMatrix = glm::scale(modelMatrix, transform.scale);

    return modelMatrix;
}

HNCRSP_NAMESPACE_END