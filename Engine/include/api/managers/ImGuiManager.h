#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/systems/Renderer.h"


HNCRSP_NAMESPACE_START

class ImGuiManager
{
private:
    Renderer* m_renderer;

public:
    ImGuiManager() = default;
    ImGuiManager(const ImGuiManager&) = delete;
    ImGuiManager& operator=(const ImGuiManager&) = delete;
    ImGuiManager(ImGuiManager&&) = delete;
    ImGuiManager& operator=(ImGuiManager&&) = delete;
    ~ImGuiManager() = default;

public:
    HNCRSP_NODISCARD uint64_t StartUp();
    void ShutDown(uint64_t key);

    void Update();

private:
    void _NewFrame();
    void _Render();
};

extern ImGuiManager g_ImGuiManager;

HNCRSP_NAMESPACE_END