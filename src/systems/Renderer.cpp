#include "Renderer.h"
#include "src/utils/TracyProfile.h"
#include "src/managers/SceneManager.h"
#include "src/ecs/ECSManager.h"
#include "src/components/DrawData.h"


HNCRSP_NAMESPACE_START

Renderer::Renderer()
{
    // Creating Screen Quad ----------
    uint8_t vertex_attrib_bits = 
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
    m_screenQuad = VertexArray(
        vertex_attrib_bits,
        vertex_data,
        indices
    );

    m_callbackData = g_SceneManager.GetCallbackData();
    m_postprocessingQueue = std::make_unique<PostProcessingQueue>(
        m_callbackData->windowWidth,
        m_callbackData->windowHeight,
        const_cast<const VertexArray*>(&m_screenQuad)
    );

    g_ShaderManager.SetPostProcessingShader(
        FileSystem::Path("resources/shaders/postprocessing/ColorCorrection.glsl")
    );
    m_postprocessingQueue->AddPostprocessingPass(g_ShaderManager.GetPostProcessingShader());

    // Setting up depth maps ----------
    m_depthMap = std::make_unique<DepthMap>(  // directional shadows
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
    glDisable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_depthMap->BindDepthBuffer(DEPTH_BUFFER_TEXTURE_UNIT_INDEX);
    // m_depthMapNoAlpha->BindDepthBuffer(DEPTH_BUFFER_NO_ALPHA_TEXTURE_UNIT_INDEX);
    _RenderScenePass();

    _SortTransparentObjects();
    glEnable(GL_BLEND);
    _RenderTransparentObjectsPass();

    glDisable(GL_DEPTH_TEST);
    m_postprocessingQueue->DrawSequence(m_callbackData);

    // bind default framebuffer and clear to clean up any ImGui stuff from last frame
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::_RenderDepthPass() const
{
    ZoneScoped;

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
        uint64_t indexBufferOffset = 0;
        for (unsigned int i = 0; i < drawData.meta_data.size(); i++)
        {
            glm::mat4 model_matrix = _GetModelMatrix(transform);
            depthPassShader->SetMat4Unf("u_model", model_matrix);
            GLCall(
                glDrawElementsBaseVertex(
                    GL_TRIANGLES,
                    drawData.meta_data[i].indices_buffer_count, 
                    GL_UNSIGNED_INT,
                    reinterpret_cast<void*>(indexBufferOffset * sizeof(float)),
                    drawData.meta_data[i].mesh_vertex_offset
                )
            );
            indexBufferOffset += drawData.meta_data[i].indices_buffer_count;
        }
    }
}

void Renderer::_RenderScenePass() const
{
    ZoneScoped;
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
        const Shader* shader = drawData.materials[0]->GetShader();

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
        uint32_t indexBufferOffset = 0;
        for (uint32_t i = 0; i < drawData.meta_data.size(); i++)
        {
            Material* material = drawData.materials[drawData.meta_data[i].material_index].get();
            if (!material->IsOpaque())
            {
                indexBufferOffset += drawData.meta_data[i].indices_buffer_count;
                continue;                
            }

            albedoMap = material->GetAlbedoMap();
            roughnessMap = material->GetRoughnessMap();
            aoMap = material->GetAoMap();
            normalMap = material->GetNormalMap();
            specularMap = material->GetSpecularMap();

            uint32_t whichShader = 0;
            if (albedoMap)
            {
                albedoMap->Bind();
                whichShader |= 1 << 0;
            }
            if (roughnessMap)
            {
                roughnessMap->Bind();
                whichShader |= 1 << 1;
            }
            if (aoMap)
            {
                aoMap->Bind();
                whichShader |= 1 << 2;
            }
            if (normalMap)
            {
                albedoMap->Bind();
                whichShader |= 1 << 3;
            }
            if (specularMap)
            {
                specularMap->Bind();
                whichShader |= 1 << 4;
            }
            shader->SetUIntUnf("u_material.whichShader", whichShader);

            // TODO: Send uniform for index into texture array
            GLCall(
                glDrawElementsBaseVertex(
                    GL_TRIANGLES,
                    drawData.meta_data[i].indices_buffer_count,
                    GL_UNSIGNED_INT,
                    reinterpret_cast<void*>(indexBufferOffset * sizeof(float)),
                    drawData.meta_data[i].mesh_vertex_offset
                )
            );
            indexBufferOffset += drawData.meta_data[i].indices_buffer_count;

            if (albedoMap) albedoMap->Unbind();
            if (roughnessMap) roughnessMap->Unbind();
            if (aoMap) aoMap->Unbind();
            if (normalMap) normalMap->Unbind();
            if (specularMap) specularMap->Unbind();
        }
    }
}

void Renderer::_RenderTransparentObjectsPass() const
{
    ZoneScoped;

    GLuint shaderID = 0;

    const Texture2D* albedoMap = nullptr;
    const Texture2D* roughnessMap = nullptr;
    const Texture2D* aoMap = nullptr;
    const Texture2D* normalMap = nullptr;
    const Texture2D* specularMap = nullptr;
    for (const DelayedTransparentObjectDrawData& obj : m_transparentObjects[m_currentSceneIndex])
    {
        DrawData& drawData = g_ECSManager.GetComponent<DrawData>(obj.entityUID);
        const Shader* shader = drawData.materials[0]->GetShader();

        Transform& transform = g_ECSManager.GetComponent<Transform>(obj.entityUID);
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

        Material* material = drawData.materials[drawData.meta_data[obj.metaDataIndex].material_index].get();

        albedoMap = material->GetAlbedoMap();
        roughnessMap = material->GetRoughnessMap();
        aoMap = material->GetAoMap();
        normalMap = material->GetNormalMap();
        specularMap = material->GetSpecularMap();

        if (albedoMap) albedoMap->Bind();
        if (roughnessMap) roughnessMap->Bind();
        if (aoMap) aoMap->Bind();
        if (normalMap) normalMap->Bind();
        if (specularMap) specularMap->Bind();

        // TODO: Send uniform for index into texture array
        GLCall(
            glDrawElementsBaseVertex(
                GL_TRIANGLES,
                drawData.meta_data[obj.metaDataIndex].indices_buffer_count,
                GL_UNSIGNED_INT,
                reinterpret_cast<void*>(obj.indexBufferOffset * sizeof(float)),
                drawData.meta_data[obj.metaDataIndex].mesh_vertex_offset
            )
        );

        if (albedoMap) albedoMap->Unbind();
        if (roughnessMap) roughnessMap->Unbind();
        if (aoMap) aoMap->Unbind();
        if (normalMap) normalMap->Unbind();
        if (specularMap) specularMap->Unbind();
    }
}

void Renderer::_SortTransparentObjects() const
{
    ZoneScopedN("Sort Transparent");
    
    // Utilities
    static const glm::vec3& cameraPos = m_camera->position;
    static std::function<bool(const DelayedTransparentObjectDrawData&, const DelayedTransparentObjectDrawData&)> comp =
        [](const DelayedTransparentObjectDrawData& obj1, const DelayedTransparentObjectDrawData& obj2) {
            glm::vec3& obj1Position = g_ECSManager.GetComponent<Transform>(obj1.entityUID).position;
            glm::vec3& obj2Position = g_ECSManager.GetComponent<Transform>(obj2.entityUID).position;
            return glm::length(obj1Position - cameraPos) < glm::length(obj2Position - cameraPos);
        };

    // Quicksort
    std::sort(m_transparentObjects[m_currentSceneIndex].begin(), m_transparentObjects[m_currentSceneIndex].end(), comp);
}

// TODO: quaternions
glm::mat4 Renderer::_GetModelMatrix(Transform& transform) const 
{
    static const Camera* camera = g_ECSManager.GetSystem<Renderer>()->GetCamera();

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, transform.position);
    
    glm::quat quaternion = glm::quat(transform.eulerAngles);
    glm::mat4 rotationMatrix = glm::toMat4(quaternion);
    if (transform.lookAtCamera)
    {
        glm::vec3 vecToCam = glm::normalize(camera->position - transform.position);
        glm::quat rota = glm::quatLookAtLH(vecToCam, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix *= glm::mat4_cast(rota);
    }
    else
    {
        modelMatrix *= rotationMatrix;
    }

    modelMatrix = glm::scale(modelMatrix, transform.scale);

    return modelMatrix;
}

void Renderer::SetCamera(Camera* camera)
{
    m_camera = camera;
}

const Camera* Renderer::GetCamera() const
{
    return m_camera;
}

Camera* Renderer::GetCameraMutable()
{
    return m_camera;
}

void Renderer::SetCubemap(const Cubemap* cubemap)
{
    m_currentCubemap = cubemap;
}

// Called from SystemManager to add related entities
void Renderer::AddEntityUID(ECS::EntityUID entity_UID)
{
    while (m_currentSceneIndex + 1 > m_shaderIDs_Order.size())
    {
        m_shaderIDs_Order.push_back({});
    }
    while (m_currentSceneIndex + 1 > m_transparentObjects.size())
    {
        m_transparentObjects.push_back({});
    }

    AddTransparentObject(entity_UID);

    // Binary-search + insert to optimize shader use
    GLuint shaderID = 
        g_ECSManager.GetComponent<DrawData>(entity_UID).materials[0]->GetShader()->GetID();
    _BinaryInsert_ShaderComparator(
        p_entityUIDs, m_shaderIDs_Order[m_currentSceneIndex], entity_UID, shaderID
    );
}

void Renderer::AddTransparentObject(ECS::EntityUID entity_UID)
{
    // Add transparent objects to m_transparentObjects
    DrawData& drawData = g_ECSManager.GetComponent<DrawData>(entity_UID);
    uint32_t indexBufferOffset = 0;
    for (uint32_t i = 0; i < drawData.meta_data.size(); i++)
    {
        const Material* material = drawData.materials[drawData.meta_data[i].material_index].get();
        if (!material->IsOpaque())
        {
            m_transparentObjects[m_currentSceneIndex].emplace_back(entity_UID, i, indexBufferOffset);
        }
        indexBufferOffset += drawData.meta_data[i].indices_buffer_count;
    }
}

void Renderer::SceneChanged(uint32_t target_scene)
{
    m_currentSceneIndex = target_scene;
}

GLuint Renderer::GetColorBufferTextureID() const
{
    return m_postprocessingQueue->GetCurrentFramebufferColorTexture();
}

void Renderer::_BinaryInsert_ShaderComparator(
    std::vector<ECS::EntityUID>& vec,
    std::vector<GLuint>& shader_list,
    ECS::EntityUID value,
    GLuint comparator
) {
    if (vec.size() == 0)
    {
        vec.push_back(value);
        shader_list.push_back(comparator);
        return;
    }
    else if (vec.size() == 1)
    {
        size_t pos = static_cast<size_t>(comparator > shader_list[0]);
        vec.insert(vec.begin() + pos, value);
        shader_list.insert(shader_list.begin() + pos, comparator);
        return;
    }

    size_t left = 0, right = vec.size() - 1;
    size_t mid = (left + right) / 2;

    while (!(shader_list[mid] <= comparator && comparator <= shader_list[mid + 1]))
    {
        if (comparator < shader_list[mid]) right = mid;
        else left = mid + 1;

        mid = (left + right) / 2;

        if (mid == vec.size() - 1) break;
        else if (mid == 0) break;
    }

    vec.insert(vec.begin() + mid + (size_t)(mid != 0), value);
    shader_list.insert(shader_list.begin() + mid + (size_t)(mid != 0), comparator);
}

void Renderer::_BinaryDelete_WShaderList(
    std::vector<ECS::EntityUID>& vec,
    std::vector<GLuint>& shader_list,
    ECS::EntityUID value
) {
    if (vec.size() == 0)
    {
        HNCRSP_TERMINATE("Vector is empty");
    }

    size_t left = 0, right = vec.size() - 1;
    size_t mid = (left + right) / 2;

    while (value != vec[mid])
    {
        if (value < vec[mid]) right = mid - 1;
        else left = mid + 1;

        mid = (left + right) / 2;
    }

    vec.erase(vec.begin() + mid);
    shader_list.erase(shader_list.begin() + mid);
}

HNCRSP_NAMESPACE_END