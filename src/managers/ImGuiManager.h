#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class ImGuiManager
{
public:
    ImGuiManager() = default;
    ImGuiManager(const ImGuiManager&) = delete;
    ImGuiManager& operator=(const ImGuiManager&) = delete;
    ImGuiManager(ImGuiManager&&) = delete;
    ImGuiManager& operator=(ImGuiManager&&) = delete;
    ~ImGuiManager() = default;

public:
    void StartUp();
    void ShutDown();

    void Update();
    void Render();
};

extern ImGuiManager g_ImGuiManager;

HNCRSP_NAMESPACE_END