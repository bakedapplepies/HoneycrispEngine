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
    for (const EntityUID& uid : entityUIDs)
    {
        MeshData& meshData = g_ECSManager->GetComponent<MeshData>(uid);
        std::shared_ptr<Material> material = meshData.material;
        std::shared_ptr<Shader> shader = material->getShader();

        Transform& transform = g_ECSManager->GetComponent<Transform>(uid);

        GLCall(glBindVertexArray(meshData.VAO_id));
        meshData.material->getShader()->Use();

        if (const Texture2D* albedoMap = material->getAlbedoMap())  // TODO: optimize this mess
        {
            albedoMap->Bind();
        }
        else
        {
            albedoMap->Unbind();
        }
        if (const Texture2D* roughnessMap = material->getRoughnessMap())
        {
            roughnessMap->Bind();
        }
        else
        {
            roughnessMap->Unbind();
        }
        if (const Texture2D* aoMap = material->getAoMap())
        {
            aoMap->Bind();
        }
        else
        {
            aoMap->Unbind();
        }
        if (const Texture2D* normalMap = material->getNormalMap())
        {
            normalMap->Bind();
        }
        else
        {
            normalMap->Unbind();
        }
        if (const Texture2D* specularMap = material->getSpecularMap())
        {
            specularMap->Bind();
        }
        else
        {
            specularMap->Unbind();
        }

        glm::mat4 modelMatrix = GetModelMatrix(transform);
        shader->setMat3Unf("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix))));
        shader->setMat4Unf("u_model", modelMatrix);
        GLCall(glDrawElements(GL_TRIANGLES, meshData.num_vertices, GL_UNSIGNED_INT, nullptr));
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