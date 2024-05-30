#include "Renderer.h"
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
    m_postprocessing_queue = std::make_unique<PostProcessingQueue>(
        m_callbackData->windowWidth * (1.0f - m_callbackData->settingsWidthPercentage),
        m_callbackData->windowHeight,
        m_screenQuad.get()
    );

    // g_ShaderManager.SetPostProcessingShader(
    //     FileSystem::Path("resources/shaders/postprocessing/Inverse.glsl")
    // );
    // m_postprocessing_queue->AddPostprocessingPass(g_ShaderManager.GetPostProcessingShader());
    // g_ShaderManager.SetPostProcessingShader(
    //     FileSystem::Path("resources/shaders/postprocessing/Blur.glsl")
    // );
    // m_postprocessing_queue->AddPostprocessingPass(g_ShaderManager.GetPostProcessingShader());
    // g_ShaderManager.SetPostProcessingShader(
    //     FileSystem::Path("resources/shaders/postprocessing/Inverse.glsl")
    // );
    // m_postprocessing_queue->AddPostprocessingPass(g_ShaderManager.GetPostProcessingShader());
    g_ShaderManager.SetPostProcessingShader(
        FileSystem::Path("resources/shaders/postprocessing/GammaCorrection.glsl")
    );
    m_postprocessing_queue->AddPostprocessingPass(g_ShaderManager.GetPostProcessingShader());

    // Setting up depth map ----------
    m_depthMap = std::make_unique<DepthMap>(
        m_callbackData->windowWidth * (1.0f - m_callbackData->settingsWidthPercentage),
        m_callbackData->windowHeight
    );
}

void Renderer::Render() const
{
    // RenderDepthPass();
    RenderScenePass();
}

void Renderer::RenderDepthPass() const
{   
    glEnable(GL_DEPTH_TEST);
    m_depthMap->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    const Shader* depthPassShader = g_ShaderManager.depthPassShader;
    depthPassShader->Use();

    // Configure shader
    glm::mat4 view_projection_matrix =
        m_depthPassCamera.GetProjectionMatrix() * m_depthPassCamera.GetViewMatrix(m_callbackData->dirLightPos, glm::vec3(0.0f));
    depthPassShader->setMat4Unf("u_depthSpaceMatrix", view_projection_matrix);

    for (const EntityUID& uid : entityUIDs)
    {
        // Get Data
        DrawData& drawData = g_ECSManager->GetComponent<DrawData>(uid);
        Transform& transform = g_ECSManager->GetComponent<Transform>(uid);

        // Draw
        glBindVertexArray(drawData.VAO_id);
        uint64_t index_buffer_offset = 0;
        for (unsigned int i = 0; i < drawData.meta_data.size(); i++)
        {
            glm::mat4 model_matrix = GetModelMatrix(transform);
            depthPassShader->setMat4Unf("u_model", model_matrix);
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

void Renderer::RenderScenePass() const
{
    m_postprocessing_queue->BindInitialFramebuffer();

    glEnable(GL_DEPTH_TEST);

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
        const Shader* shader = drawData.materials[0]->getShader();

        Transform& transform = g_ECSManager->GetComponent<Transform>(uid);
        glBindVertexArray(drawData.VAO_id);

        if (shaderID != shader->getID())
        {
            shader->Use();
            shaderID = shader->getID();
        }

        glm::mat4 model_matrix = GetModelMatrix(transform);
        shader->setMat3Unf("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(model_matrix))));
        shader->setMat4Unf("u_model", model_matrix);
        uint64_t index_buffer_offset = 0;
        for (unsigned int i = 0; i < drawData.meta_data.size(); i++)
        {
            Material* material = drawData.materials[drawData.meta_data[i].material_index].get();
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

    glDisable(GL_DEPTH_TEST);

    m_postprocessing_queue->DrawSequence(m_callbackData);
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

// for ECS
void Renderer::AddEntityUID(EntityUID entityUID)
{
    GLuint shaderID = 
        g_ECSManager->GetComponent<DrawData>(entityUID).materials[0]->getShader()->getID();
    binary_insert_shader_comparator(entityUIDs, m_shaderIDs_Order, entityUID, shaderID);
}

HNCRSP_NAMESPACE_END