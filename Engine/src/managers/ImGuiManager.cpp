#include "api/managers/ImGuiManager.h"
#include "vendor/imgui/implot.h"
#include "api/ecs/ECSManager.h"
#include "api/managers/SceneManager.h"
#include "api/managers/ShaderManager.h"
#include "src/WindowHandler.h"
#include "api/core/Random.h"
#include "api/core/Timer.h"


HNCRSP_NAMESPACE_START

ImGuiManager g_ImGuiManager;

void ImGuiManager::Update(uint32_t last_frame_FPS)
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
    static std::vector<double> fpsData;
    static std::vector<double> fpsDataTimeFrame;
    static double baseTime = std::ceilf(ImGui::GetTime());
    if (ImGui::GetTime() >= baseTime)
    {
        fpsData.push_back(static_cast<double>(last_frame_FPS));
        fpsDataTimeFrame.push_back(baseTime);
        if (fpsData.size() >= 21)
        {
            fpsData.erase(fpsData.begin());
            fpsDataTimeFrame.erase(fpsDataTimeFrame.begin());
        }
        baseTime += 0.5;
    }
    if (ImPlot::BeginPlot("Line Plots"))
    {
        ImPlot::SetNextAxisLimits(ImAxis_Y1, 0.0, 2000.0);
        ImPlot::SetNextAxisToFit(ImAxis_X1);
        ImPlot::SetNextAxisToFit(ImAxis_Y1);
        ImPlot::SetupAxes("Time","FPS");
        ImPlot::PlotLine("FPS", fpsDataTimeFrame.data(), fpsData.data(), fpsData.size());
        ImPlot::EndPlot();
    } 
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

    ImGui::Begin("Depth Map");
    GLuint depthBufferTextureID = m_renderer->GetDepthBufferTextureID();
    ImGui::Image(
        reinterpret_cast<ImTextureID>(depthBufferTextureID),
        ImGui::GetContentRegionAvail(),
        ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f)
    );
    ImGui::End();

    _Render();
}

void ImGuiManager::_NewFrame()
{
    // Timer<std::chrono::milliseconds> timer("ImGui new frame");
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
    ImPlot::CreateContext();
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
        HNCRSP_TERMINATE("ImGuiManager shutdown key invalid!");
        return;
    }
    ImGui::Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();
}

HNCRSP_NAMESPACE_END