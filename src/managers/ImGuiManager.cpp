#include "ImGuiManager.h"


HNCRSP_NAMESPACE_START

ImGuiManager g_ImGuiManager;

void ImGuiManager::StartUp()
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);

    ImGui::StyleColorsDark();
    // ImGui::GetStyle().ScaleAllSizes(2.0f);
}

void ImGuiManager::ShutDown()
{
    ImGui::Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

HNCRSP_NAMESPACE_END