#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/graphics/core/Shader.h"


HNCRSP_NAMESPACE_START

class DepthMap
{
private:
    GLuint m_framebufferID;
    GLuint m_depthTexture_ID;
    int m_width;
    int m_height;

    const Shader* m_shader;

public:
    DepthMap(int width, int height);
    ~DepthMap();

    void Bind() const;
    void Unbind() const;
    void BindDepthBuffer(const uint16_t texture_unit) const;
};

HNCRSP_NAMESPACE_END