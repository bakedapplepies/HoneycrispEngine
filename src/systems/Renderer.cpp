#include "Renderer.h"
#include "src/utils/TracyProfile.h"
#include "src/managers/SceneManager.h"
#include "src/ecs/ECSManager.h"
#include "src/components/DrawData.h"


HNCRSP_NAMESPACE_START

Renderer::Renderer()
{
    // Creating Screen Quad ----------
    unsigned short vertex_attrib_bits = 
        VERTEX_ATTRIB_POSITION_BIT | VERTEX_ATTRIB_UV_BIT;

    std::vector<float> vertex_data = {
        // positions          uvs
        -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    };
    std::vector<GLuint> indices = {
        0, 1, 3,
        1, 2, 3
    };
    m_screenQuad = std::make_unique<VertexArray>(
        vertex_attrib_bits,
        vertex_data,
        indices
    );

    m_callbackData = g_SceneManager.GetCallbackData();
    m_postprocessingQueue = std::make_unique<PostProcessingQueue>(
        m_callbackData->windowWidth,
        m_callbackData->windowHeight,
        m_screenQuad.get()
    );

    g_ShaderManager.SetPostProcessingShader(
        FileSystem::Path("resources/shaders/postprocessing/ColorCorrection.glsl")
    );
    m_postprocessingQueue->AddPostprocessingPass(g_ShaderManager.GetPostProcessingShader());

    // Setting up depth map ----------
    m_depthMap = std::make_unique<DepthMap>(
        m_callbackData->windowWidth,
        m_callbackData->windowHeight
    );
}

// General pipeline here
void Renderer::Render() const
{
    ZoneScopedN("Render");

    m_depthMap->Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    _RenderDepthPass();

    m_postprocessingQueue->BindInitialFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_depthMap->BindDepthBuffer();
    _RenderScenePass();

    glDisable(GL_DEPTH_TEST);
    m_postprocessingQueue->DrawSequence(m_callbackData);

    // bind default framebuffer and clear to clean up any ImGui stuff from last frame
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::_RenderDepthPass() const
{
    const Shader* depthPassShader = g_ShaderManager.depthPassShader;
    depthPassShader->Use();

    // Configure shader
    glm::mat4 view_projection_matrix = m_depthPassCamera.GetViewProjectionMatrix(m_callbackData->dirLightPos, glm::vec3(0.0f));
    depthPassShader->SetMat4Unf("u_depthSpaceMatrix", view_projection_matrix);

    for (const ECS::EntityUID& uid : p_entityUIDs)
    {
        // Get Data
        DrawData& drawData = g_ECSManager.GetComponent<DrawData>(uid);
        Transform& transform = g_ECSManager.GetComponent<Transform>(uid);

        // Draw
        glBindVertexArray(drawData.VAO_id);
        uint64_t index_buffer_offset = 0;
        for (unsigned int i = 0; i < drawData.meta_data.size(); i++)
        {
            glm::mat4 model_matrix = _GetModelMatrix(transform);
            depthPassShader->SetMat4Unf("u_model", model_matrix);
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
    }
}

void Renderer::_RenderScenePass() const
{
    GLuint shaderID = 0;
    if (m_currentCubemap)
    {
        m_currentCubemap->Draw();
    }

    const Texture2D* albedoMap = nullptr;
    const Texture2D* roughnessMap = nullptr;
    const Texture2D* aoMap = nullptr;
    const Texture2D* normalMap = nullptr;
    const Texture2D* specularMap = nullptr;
    for (const ECS::EntityUID& uid : p_entityUIDs)
    {
        DrawData& drawData = g_ECSManager.GetComponent<DrawData>(uid);
        const Shader* shader = drawData.materials[0]->getShader();

        Transform& transform = g_ECSManager.GetComponent<Transform>(uid);
        glBindVertexArray(drawData.VAO_id);

        if (shaderID != shader->GetID())
        {
            shader->Use();
            shaderID = shader->GetID();
        }

        glm::mat4 model_matrix = _GetModelMatrix(transform);
        shader->SetMat4Unf("u_depthSpaceMatrix", m_depthPassCamera.GetViewProjectionMatrix(m_callbackData->dirLightPos, glm::vec3(0.0f)));
        shader->SetMat3Unf("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(model_matrix))));
        shader->SetMat4Unf("u_model", model_matrix);
        uint64_t index_buffer_offset = 0;
        for (unsigned int i = 0; i < drawData.meta_data.size(); i++)
        {
            Material* material = drawData.materials[drawData.meta_data[i].material_index].get();
            albedoMap = material->GetAlbedoMap();
            roughnessMap = material->GetRoughnessMap();
            aoMap = material->GetAoMap();
            normalMap = material->GetNormalMap();
            specularMap = material->GetSpecularMap();

            if (albedoMap) albedoMap->Bind();
            if (roughnessMap) roughnessMap->Bind();
            if (aoMap) aoMap->Bind();
            if (normalMap) albedoMap->Bind();
            if (specularMap) specularMap->Bind();

            // TODO: Send uniform for index into texture array
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

            if (albedoMap) albedoMap->Unbind();
            if (roughnessMap) roughnessMap->Unbind();
            if (aoMap) aoMap->Unbind();
            if (normalMap) albedoMap->Unbind();
            if (specularMap) specularMap->Unbind();
        }
    }
}

// TODO: quaternions
glm::mat4 Renderer::_GetModelMatrix(Transform& transform) const 
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

void Renderer::SwitchCubemap(const Cubemap* cubemap)
{
    m_currentCubemap = cubemap;
}

// Called from SystemManager to add related entities
void Renderer::AddEntityUID(ECS::EntityUID entity_UID)
{
    uint32_t currentSceneIndex = g_SceneManager.GetCurrentSceneIndex();
    while (currentSceneIndex + 1 > m_shaderIDs_Order.size())
    {
        m_shaderIDs_Order.push_back({});
    }

    GLuint shaderID = 
        g_ECSManager.GetComponent<DrawData>(entity_UID).materials[0]->getShader()->GetID();
    binary_insert_shader_comparator(
        p_entityUIDs, m_shaderIDs_Order[currentSceneIndex], entity_UID, shaderID
    );
}

GLuint Renderer::GetColorBufferTextureID() const
{
    return m_postprocessingQueue->GetCurrentFramebufferColorTexture();
}

HNCRSP_NAMESPACE_END