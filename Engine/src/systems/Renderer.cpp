#include "api/systems/Renderer.h"
#include "api/ecs/ECSManager.h"
#include "api/managers/SceneManager.h"
#include "api/managers/ShaderManager.h"
#include "src/WindowHandler.h"
#include "api/core/TracyProfile.h"
#include "api/components/DrawData.h"


HNCRSP_NAMESPACE_START

#define DEPTH_CAMERA_RESOLUTION depthCameraResolution

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

    CallbackData* callbackData = GetCallbackData();
    m_postprocessingQueue = std::make_unique<PostProcessingQueue>(
        callbackData->windowWidth,
        callbackData->windowHeight,
        const_cast<const VertexArray*>(&m_screenQuad)
    );

    // Only one post-processing shader for now
    g_ShaderManager.SetPostProcessingShader(
        FileSystem::Path("resources/shaders/postprocessing/ColorCorrection.frag")
    );
    m_postprocessingQueue->AddPostprocessingPass(g_ShaderManager.GetPostProcessingShader());

    // Setting up depth maps ----------
    m_depthMap = std::make_unique<DepthMap>(  // directional shadows
        callbackData->windowWidth,
        callbackData->windowHeight
    );
}

// General pipeline here
void Renderer::_Render() const
{
    ZoneScopedN("Render");

    // Depth pass (directional shadow)
    glEnable(GL_DEPTH_TEST);
    m_depthMap->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    // glCullFace(GL_FRONT);
    _RenderDepthPass();
    // glCullFace(GL_BACK);

    // Scene pass (no transparent objects)
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_postprocessingQueue->BindInitialFramebuffer();
    glDisable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_depthMap->BindDepthTexture(DEPTH_BUFFER_TEXTURE_UNIT_INDEX);
    _RenderScenePass();
    
    // Scene pass (transparent objects)
    _SortTransparentObjects();
    glEnable(GL_BLEND);
    _RenderTransparentObjectsPass();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_DEPTH_TEST);
    m_postprocessingQueue->DrawSequence();

    // m_axesCrosshair.Render();

    // bind default framebuffer and clear to clean up any ImGui stuff from last frame
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::_RenderDepthPass() const
{
    ZoneScoped;

    const Shader* depthPassShader = g_ShaderManager.depthPassShader;
    depthPassShader->Use();

    // Configure shader
    const DirectionalLight* dirLight = g_SceneManager.GetCurrentDirectionalLight();

    glm::mat4 viewProjectionMatrix;
    if (dirLight)
    {
        m_depthPassCamera.SetDirection(dirLight->direction);
    }
    else
    {
        m_depthPassCamera.SetDirection(VEC3_DOWN);
    }

    // HNCRSP_INFO("{}", dirLight->direction);
    glm::vec3 targetDir = glm::vec3(m_camera->direction.x, 0.0f, m_camera->direction.z);
    glm::vec3 targetPos = m_camera->direction * 10.0f;
    viewProjectionMatrix = m_depthPassCamera.GetViewProjectionMatrix(targetPos, DEPTH_CAMERA_RESOLUTION);
    depthPassShader->SetMat4Unf("u_depthSpaceMatrix", viewProjectionMatrix);

    for (const ECS::EntityUID& uid : p_entityUIDs)
    {
        // Get Data
        const DrawData& drawData = g_ECSManager.GetComponent<DrawData>(uid);
        const Transform& transform = g_ECSManager.GetComponent<Transform>(uid);

        // Draw
        glBindVertexArray(drawData.VAO_id);
        uint64_t indexBufferOffset = 0;
        for (unsigned int i = 0; i < drawData.meta_data.size(); i++)
        {
            glm::mat4 model_matrix = _GetModelMatrix(transform);
            depthPassShader->SetMat4Unf("u_model", model_matrix);
            glDrawElementsBaseVertex(
                GL_TRIANGLES,
                drawData.meta_data[i].indices_buffer_count, 
                GL_UNSIGNED_INT,
                reinterpret_cast<void*>(indexBufferOffset * sizeof(float)),
                drawData.meta_data[i].mesh_vertex_offset
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
        const DrawData& drawData = g_ECSManager.GetComponent<DrawData>(uid);
        const Shader* shader = drawData.materials[0].GetShader();

        const Transform& transform = g_ECSManager.GetComponent<Transform>(uid);
        glBindVertexArray(drawData.VAO_id);

        if (shaderID != shader->GetID())
        {
            shader->Use();
            shaderID = shader->GetID();
        }

        glm::mat4 model_matrix = _GetModelMatrix(transform);

        // Configure shader
        const DirectionalLight* dirLight = g_SceneManager.GetCurrentDirectionalLight();

        glm::mat4 viewProjectionMatrix;
        glm::vec3 targetDir = glm::vec3(m_camera->direction.x, 0.0f, m_camera->direction.z);
        glm::vec3 targetPos = m_camera->direction * 10.0f;
        viewProjectionMatrix = m_depthPassCamera.GetViewProjectionMatrix(targetPos, DEPTH_CAMERA_RESOLUTION);        shader->SetMat4Unf("u_depthSpaceMatrix", viewProjectionMatrix);
        shader->SetMat3Unf("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(model_matrix))));
        shader->SetMat4Unf("u_model", model_matrix);
        uint32_t indexBufferOffset = 0;
        for (uint32_t i = 0; i < drawData.meta_data.size(); i++)
        {
            const Material& material = drawData.materials[drawData.meta_data[i].material_index];
            if (!material.IsOpaque())
            {
                
                indexBufferOffset += drawData.meta_data[i].indices_buffer_count;
                continue;                
            }

            albedoMap = material.GetAlbedoMap();
            roughnessMap = material.GetRoughnessMap();
            aoMap = material.GetAoMap();
            normalMap = material.GetNormalMap();
            specularMap = material.GetSpecularMap();

            uint32_t whichMaterial = 0;
            if (albedoMap)
            {
                albedoMap->Bind();
                whichMaterial |= 1 << 0;
            }
            if (roughnessMap)
            {
                roughnessMap->Bind();
                whichMaterial |= 1 << 1;
            }
            if (aoMap)
            {
                aoMap->Bind();
                whichMaterial |= 1 << 2;
            }
            if (normalMap)
            {
                normalMap->Bind();
                whichMaterial |= 1 << 3;
            }
            if (specularMap)
            {
                specularMap->Bind();
                whichMaterial |= 1 << 4;
            }
            shader->SetUIntUnf("u_material.whichMaterial", whichMaterial);

            // TODO: Send uniform for index into texture array
            glDrawElementsBaseVertex(
                GL_TRIANGLES,
                drawData.meta_data[i].indices_buffer_count,
                GL_UNSIGNED_INT,
                reinterpret_cast<void*>(indexBufferOffset * sizeof(float)),
                drawData.meta_data[i].mesh_vertex_offset
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
        const DrawData& drawData = g_ECSManager.GetComponent<DrawData>(obj.entityUID);
        const Shader* shader = drawData.materials[0].GetShader();

        const Transform& transform = g_ECSManager.GetComponent<Transform>(obj.entityUID);
        glBindVertexArray(drawData.VAO_id);

        if (shaderID != shader->GetID())
        {
            shader->Use();
            shaderID = shader->GetID();
        }

        glm::mat4 model_matrix = _GetModelMatrix(transform);

        // Configure shader
        const DirectionalLight* dirLight = g_SceneManager.GetCurrentDirectionalLight();

        glm::mat4 viewProjectionMatrix;
        glm::vec3 targetDir = glm::vec3(m_camera->direction.x, 0.0f, m_camera->direction.z);
        glm::vec3 targetPos = m_camera->direction * 10.0f;
        viewProjectionMatrix = m_depthPassCamera.GetViewProjectionMatrix(targetPos, DEPTH_CAMERA_RESOLUTION);        shader->SetMat4Unf("u_depthSpaceMatrix", viewProjectionMatrix);
        shader->SetMat3Unf("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(model_matrix))));
        shader->SetMat4Unf("u_model", model_matrix);

        const Material& material = drawData.materials[drawData.meta_data[obj.metaDataIndex].material_index];

        albedoMap = material.GetAlbedoMap();
        roughnessMap = material.GetRoughnessMap();
        aoMap = material.GetAoMap();
        normalMap = material.GetNormalMap();
        specularMap = material.GetSpecularMap();

        uint32_t whichMaterial = 0;
        if (albedoMap)
        {
            albedoMap->Bind();
            whichMaterial |= 1 << 0;
        }
        if (roughnessMap)
        {
            roughnessMap->Bind();
            whichMaterial |= 1 << 1;
        }
        if (aoMap)
        {
            aoMap->Bind();
            whichMaterial |= 1 << 2;
        }
        if (normalMap)
        {
            normalMap->Bind();
            whichMaterial |= 1 << 3;
        }
        if (specularMap)
        {
            specularMap->Bind();
            whichMaterial |= 1 << 4;
        }
        shader->SetUIntUnf("u_material.whichMaterial", whichMaterial);

        // TODO: Send uniform for index into texture array
        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            drawData.meta_data[obj.metaDataIndex].indices_buffer_count,
            GL_UNSIGNED_INT,
            reinterpret_cast<void*>(obj.indexBufferOffset * sizeof(float)),
            drawData.meta_data[obj.metaDataIndex].mesh_vertex_offset
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
    
    // Comparison function
    static const glm::vec3& cameraPos = m_camera->position;
    static std::function<bool(const DelayedTransparentObjectDrawData&, const DelayedTransparentObjectDrawData&)> comp =
        [](const DelayedTransparentObjectDrawData& obj1, const DelayedTransparentObjectDrawData& obj2) {
            glm::vec3& obj1Position = g_ECSManager.GetComponent<Transform>(obj1.entityUID).position;
            glm::vec3& obj2Position = g_ECSManager.GetComponent<Transform>(obj2.entityUID).position;
            glm::vec3 camToObj1 = obj1Position - cameraPos;
            glm::vec3 camToObj2 = obj2Position - cameraPos;
            float len1 = (camToObj1).x * (camToObj1).x +
                         (camToObj1).y * (camToObj1).y +
                         (camToObj1).z * (camToObj1).z;
            float len2 = (camToObj2).x * (camToObj2).x +
                         (camToObj2).y * (camToObj2).y +
                         (camToObj2).z * (camToObj2).z;
                    
            return len1 < len2;
        };

    // Quicksort
    std::sort(m_transparentObjects[m_currentSceneIndex].begin(), m_transparentObjects[m_currentSceneIndex].end(), comp);
}

// TODO: quaternions
glm::mat4 Renderer::_GetModelMatrix(const Transform& transform) const 
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

void Renderer::SetDepthPassCamDistFromMainCam(float dist) const
{
    m_depthPassCamera.distOffMainCam = dist;
}

void Renderer::SetCubemap(const Cubemap* cubemap)
{
    m_currentCubemap = cubemap;
}

// Called from SystemManager to add related entities
void Renderer::AddEntityUID(ECS::EntityUID entity_UID)
{
    // TODO: When adding new systems, this would be very troublesome
    // Add new object slots for new scenes
    while (m_currentSceneIndex + 1 > m_shaderIDs_Order.size())
    {
        m_shaderIDs_Order.push_back({});
    }
    while (m_currentSceneIndex + 1 > m_transparentObjects.size())
    {
        m_transparentObjects.push_back({});
    }

    AddTransparentObject(entity_UID);

    // Binary-search + insert with shader ID to optimize shader use
    GLuint shaderID = 
        g_ECSManager.GetComponent<DrawData>(entity_UID).materials[0].GetShader()->GetID();
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
        const Material& material = drawData.materials[drawData.meta_data[i].material_index];
        if (!material.IsOpaque())
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

GLuint Renderer::GetDepthBufferTextureID() const
{
    return m_depthMap->GetTextureID();
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