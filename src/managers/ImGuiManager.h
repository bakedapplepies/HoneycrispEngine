#pragma once

#include "src/pch/pch.h"


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
};

extern ImGuiManager g_ImGuiManager;

HNCRSP_NAMESPACE_END