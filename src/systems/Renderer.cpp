#include "Renderer.h"
#include "src/ecs/ECSManager.h"
#include "src/components/DrawData.h"


HNCRSP_NAMESPACE_START

void Renderer::StartUp()
{
}

void Renderer::Render() const
{
    GLuint shaderID = 0;
    if (auto cubemap = m_weak_currentCubemap.lock())  // TODO: slow?
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
        DrawData& drawData = g_ECSManager->GetComponent<DrawData>(uid);
        Material* material = drawData.material.get();
        Shader* shader = material->getShader().get();

        Transform& transform = g_ECSManager->GetComponent<Transform>(uid);
        glBindVertexArray(drawData.VAO_id);

        if (shaderID != shader->getID())
        {
            shader->Use();
            shaderID = shader->getID();
            // HNCRSP_LOG_INFO(shader->getID());
        }

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
        uint64_t index_buffer_offset = 0;
        for (unsigned int i = 0; i < drawData.meta_data.size(); i++)
        {
            GLCall(
                glDrawElementsBaseVertex(
                    GL_TRIANGLES,
                    drawData.meta_data[i].indices_buffer_count,
                    GL_UNSIGNED_INT,
                    reinterpret_cast<void*>(index_buffer_offset * sizeof(float)),
                    drawData.meta_data[i].mesh_vertex_count
                )
            );
            index_buffer_offset += drawData.meta_data[i].indices_buffer_count;
        }

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
    GLuint shaderID = g_ECSManager->GetComponent<DrawData>(entityUID).material->getShader()->getID();
    binary_insert_shader_comparator(entityUIDs, m_shaderIDs_Order, entityUID, shaderID);
}

HNCRSP_NAMESPACE_END