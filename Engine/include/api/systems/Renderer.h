#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/ecs/System.h"
#include "api/components/Transform.h"

#include "api/graphics/Camera.h"
#include "api/graphics/Cubemap.h"
#include "api/graphics/core/DepthMap.h"
#include "api/graphics/DepthPassCamera.h"
#include "api/graphics/PostProcessingQueue.h"
#include "api/graphics/AxesCrosshair.h"

HNCRSP_NAMESPACE_START

struct DelayedTransparentObjectDrawData;  // end of file
struct DrawElementsIndirectCommand;  // end of file

class GameLoop;
class Renderer : public ECS::System
{
friend GameLoop;
private:
    Camera* m_camera = nullptr;
    const Cubemap* m_currentCubemap = nullptr;
    uint32_t m_currentSceneIndex;

    std::vector< std::vector<GLuint> > m_shaderIDs_Order;  // only used when adding new entities
    mutable std::vector< std::vector<DelayedTransparentObjectDrawData> > m_transparentObjects;
    VertexArray m_screenQuad;

    std::unique_ptr<PostProcessingQueue> m_postprocessingQueue;
    std::unique_ptr<DepthMap> m_depthMap;
    mutable DepthPassCamera m_depthPassCamera;

    // gizmos
    AxesCrosshair m_axesCrosshair;

public:
    Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;
    ~Renderer() = default;

    void SetCubemap(const Cubemap* cubemap);
    void SetCamera(Camera* camera);
    const Camera* GetCamera() const;
    Camera* GetCameraMutable();
    void SetDepthPassCamDistFromMainCam(float dist) const;

    GLuint GetColorBufferTextureID() const;
    GLuint GetDepthBufferTextureID() const;
    void AddTransparentObject(ECS::EntityUID entity_UID);

private:
    // ECS functions
    void AddEntityUID(ECS::EntityUID entity_UID) override;
    void SceneChanged(uint32_t target_scene) override;

    // Render Passes
    void _RenderDepthPass() const;
    void _RenderScenePass() const;
    void _RenderTransparentObjectsPass() const;
    void _Render() const;

    // Utility functions
    glm::mat4 _GetModelMatrix(Transform& transform) const;
    void _SortTransparentObjects() const;
    void _GenerateDrawCommands();
    void _BinaryInsert_ShaderComparator(
        std::vector<ECS::EntityUID>& vec,
        std::vector<GLuint>& shader_list,
        ECS::EntityUID value,
        GLuint comparator
    );
    void _BinaryDelete_WShaderList(
        std::vector<ECS::EntityUID>& vec,
        std::vector<GLuint>& shader_list,
        ECS::EntityUID value
    );
};

struct DelayedTransparentObjectDrawData
{
    ECS::EntityUID entityUID;
    uint32_t metaDataIndex;
    uint32_t indexBufferOffset;
};

struct DrawElementsIndirectCommand
{
    GLuint count = 1;
    GLuint instanceCount = 1;
    GLuint firstIndex = 0;
    GLint baseVertex = 0;
    GLuint baseInstance = 0;
};

HNCRSP_NAMESPACE_END