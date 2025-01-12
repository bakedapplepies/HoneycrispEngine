#include "api/managers/ImGuiManager.h"
#include "api/ecs/ECSManager.h"
#include "api/managers/SceneManager.h"
#include "src/WindowHandler.h"
#include "api/managers/ShaderManager.h"
#include "api/core/Random.h"


HNCRSP_NAMESPACE_START

ImGuiManager g_ImGuiManager;

void ImGuiManager::Update()
{
    static CallbackData* callbackData = GetCallbackData();

    _NewFrame();

    // ----- Global settings -----
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(callbackData->windowWidth * 0.2f, callbackData->windowHeight * 0.5f), ImGuiCond_Always);
    ImGui::Begin("Global settings");
    ImGui::GetCurrentWindow()->FontWindowScale = callbackData->windowHeight / 1080.0f;  // scale ui based on window height, might make it decay exponentially
    
    ImGui::SliderFloat("Camera Speed",
                        &m_renderer->GetCameraMutable()->speed,
                        2.0f, 20.0f);

    static float depthPassCamDistFromMainCam = 50.0f;
    if (ImGui::SliderFloat("Depth-pass Camera's Distance from Main Camera",
                            &depthPassCamDistFromMainCam,
                            1.0f, 100.0f))
    {
        m_renderer->SetDepthPassCamDistFromMainCam(depthPassCamDistFromMainCam);
    }
    
    ImGui::NewLine();
    ImGui::SeparatorText("Post-processing effects variables");

    const Shader* postprocessingShader = g_ShaderManager.GetPostProcessingShader();
    static float     brightness   = 0.0f;
    static float     contrast     = 0.0f;  // -100.0 - 100.0
    static float     saturation   = 1.0f;  // -100.0 - 100.0
    static float     postExposure = 0.0f;
    static float     temperature  = 0.0f;
    static float     tint         = 0.0f;
    static float     cwhite       = 60.0f;
    static glm::vec3 colorFilter  = glm::vec3(1.0f);  // white
    
    if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 5.0f))
        postprocessingShader->SetFloatUnf("u_brightness", brightness);

    if (ImGui::SliderFloat("Contrast", &contrast, -100.0f, 100.0f))
        postprocessingShader->SetFloatUnf("u_contrast", contrast * 0.01f + 1.0f);

    if (ImGui::SliderFloat("Saturation", &saturation, 0.0f, 1.0f))
        postprocessingShader->SetFloatUnf("u_saturation", saturation);

    if (ImGui::DragFloat("Post Exposure", &postExposure))
        postprocessingShader->SetFloatUnf("u_postExposure", std::powf(2.0f, postExposure));

    if (ImGui::SliderFloat("Temperature", &temperature, 0.0f, 1.0f))
        postprocessingShader->SetFloatUnf("u_temperature", temperature);

    if (ImGui::SliderFloat("Tint", &tint, 0.0f, 1.0f))
        postprocessingShader->SetFloatUnf("u_tint", tint);

    if (ImGui::SliderFloat("CWhite", &cwhite, 1.0f, 120.0f))
        postprocessingShader->SetFloatUnf("u_cwhite", cwhite);

    if (ImGui::ColorPicker4("Color Filter", glm::value_ptr(colorFilter)))
        postprocessingShader->SetVec3Unf("u_colorFilter", colorFilter);

    ImGui::NewLine();
    ImGui::SeparatorText("Statistics");
    // float totalRenderTime = depthPass + scenePass + postprocessing;
    // ImGui::Text("Total Render time: %fms (%f%%)", totalRenderTime * 1000, totalRenderTime/m_deltaTime);

    // static bool anti_aliasing;
    // if (ImGui::Checkbox("Anti-aliasing", &anti_aliasing))
    // {
    //     HNCRSP_INFO(anti_aliasing);
    // }
    
    // ImGui::Image((void*)&Texture2DManager::mainTextureMap->getID(), ImVec2(100.0f, 100.0f));
    
    ImGui::End();

    // ----- Per-scene settings -----
    ImGui::SetNextWindowPos(ImVec2(0.0f, callbackData->windowHeight * 0.5f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(callbackData->windowWidth * 0.2f, callbackData->windowHeight * 0.5f), ImGuiCond_Always);
    ImGui::Begin("Scene settings");
    ImGui::GetCurrentWindow()->FontWindowScale = callbackData->windowHeight / 1080.0f;

    // ImGui Windows from scenes
    g_SceneManager._UpdateImGui();

    ImGui::End();

    // ----- Scene Viewport -----
    ImGui::SetNextWindowPos(ImVec2(callbackData->windowWidth * 0.2f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(callbackData->windowWidth * 0.8f, callbackData->windowHeight), ImGuiCond_Always);
    ImGui::Begin("Viewport");
    ImGui::GetCurrentWindow()->FontWindowScale = callbackData->windowHeight / 1080.0f;

    GLuint colorBufferTextureID = m_renderer->GetColorBufferTextureID();
    ImGui::Image(
        reinterpret_cast<ImTextureID>(colorBufferTextureID),
        ImGui::GetContentRegionAvail(),
        ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f)
    );

    ImGui::End();

    _Render();
}

void ImGuiManager::_NewFrame()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame(); 
    ImGui::NewFrame();
}

void ImGuiManager::_Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static uint64_t s_key;
uint64_t ImGuiManager::StartUp()
{
    static bool startedUp = false;
    if (startedUp)
    {
        HNCRSP_ERROR("ImGuiManager already initialized!");
        return UINT64_MAX;
    }
    startedUp = true;
    s_key = Random<uint64_t>::dist(0ULL, UINT64_MAX - 1);
    
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);

    ImGui::StyleColorsDark();
    // ImGui::GetStyle().ScaleAllSizes(2.0f);

    m_renderer = g_ECSManager.GetSystem<Renderer>();
    
    return s_key;  // key hidden by implementation
}

void ImGuiManager::ShutDown(uint64_t key)
{
    if (key != s_key)
    {
        HNCRSP_ERROR("ImGuiManager shutdown key invalid!");
        return;
    }
    ImGui::Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

HNCRSP_NAMESPACE_END