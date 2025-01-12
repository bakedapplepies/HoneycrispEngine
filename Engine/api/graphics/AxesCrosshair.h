#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/VertexArray.h"
#include "api/graphics/core/Shader.h"


HNCRSP_NAMESPACE_START

class AxesCrosshair
{
private:
    VertexArray m_VAO;
    const Shader* m_shader;

public:
    AxesCrosshair();
    AxesCrosshair(const AxesCrosshair& other) = delete;
    AxesCrosshair& operator=(const AxesCrosshair& other) = delete;
    AxesCrosshair(AxesCrosshair&& other) noexcept;
    AxesCrosshair& operator=(AxesCrosshair&& other) noexcept;
    ~AxesCrosshair() = default;

    void Render() const;
};

HNCRSP_NAMESPACE_END