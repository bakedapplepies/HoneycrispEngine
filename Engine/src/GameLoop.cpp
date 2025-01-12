#include "GameLoop.h"
#include "api/core/TracyProfile.h"
#include "api/managers/Texture2DManager.h"
#include "src/WindowHandler.h"
#include "api/managers/ImGuiManager.h"
#include "api/core/Timer.h"
#include "api/graphics/core/UniformBuffer.h"

HNCRSP_NAMESPACE_START

GameLoop::GameLoop()
{

}

void GameLoop::Start()
{
    if (!m_continueProgram) return;

    GLFWwindow* window = glfwGetCurrentContext();
    CallbackData* callbackData = GetCallbackData();

    // ----- Systems -----
    Renderer* renderer = g_ECSManager.GetSystem<Renderer>();
    
    // ----- Setting up Camera -----
    m_camera = Camera(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_camera.position += glm::vec3(0, 10, 0);
    m_camera.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    renderer->SetCamera(&m_camera);

    // ----- Window stuff -----
    float begin = glfwGetTime();

    // view matrix, ortho proj matrix, proj matrix, time
    UniformBuffer<glm::mat4, glm::mat4, glm::mat4, float> uboMatrices(0);  // UBO binding index
    // viewPos, spotlightPos, spotlightDir
    UniformBuffer<glm::vec3, glm::vec3, glm::vec3> uboOther(1);

    while(!glfwWindowShouldClose(window))
    {
        ZoneScoped;

        m_deltaTime = glfwGetTime() - begin;
        begin = glfwGetTime();

        _CalcFPS();
        _ProcessInput();

        // Update camera
        m_camera.direction = glm::normalize(m_camera.direction);

        // ----- Global uniforms -----
        float u_time = begin;
        uboMatrices.Bind();
        uboMatrices.Update(
            glm::value_ptr(m_camera.GetViewMatrix()),
            glm::value_ptr(m_camera.GetOrthoMatrix(-5.0f, 5.0f, -5.0f, 5.0f)),
            glm::value_ptr(m_camera.GetProjectionMatrix(callbackData->windowWidth * 0.8f, callbackData->windowHeight)),
            &u_time
        );
        
        uboOther.Bind();
        uboOther.Update(
            glm::value_ptr(m_camera.position),
            glm::value_ptr(m_camera.position),
            glm::value_ptr(m_camera.direction)
        );

        // ----- Update and Render scenes -----
        g_SceneManager._Update(m_deltaTime);  // update scene before rendering
        renderer->_Render();

        // ----- Render ImGui ----- always render after scene so it doesn't get drawn over
        g_ImGuiManager.Update();
                
        glfwSwapBuffers(window);
        glfwPollEvents();

        FrameMark;
    }
}

void GameLoop::_ProcessInput()
{
    ZoneScoped;

    static GLFWwindow* window = glfwGetCurrentContext();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_camera.ChangePos(glm::normalize(glm::vec3(m_camera.direction.x, 0.0f, m_camera.direction.z)) * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_camera.ChangePos(-glm::normalize(glm::vec3(m_camera.direction.x, 0.0f, m_camera.direction.z)) * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(m_camera.direction, m_camera.cameraUp));
        m_camera.ChangePos(m_camera.GetLeftVec() * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(glm::cross(m_camera.direction, m_camera.cameraUp));
        m_camera.ChangePos(m_camera.GetRightVec() * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_camera.ChangePos(glm::vec3(0.0f, 1.0f, 0.0f) * m_camera.speed * m_deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_camera.ChangePos(glm::vec3(0.0f, -1.0f, 0.0f) * m_camera.speed * m_deltaTime);
    }
}

static uint32_t lowFPS  = 0xFFFFFFFF;
static uint32_t highFPS = 0;
static uint32_t countedFPSes = 0;
static uint32_t totalFPS = 0;  // uint32_t is more than enough
void GameLoop::_CalcFPS()
{
    ZoneScoped;

    static GLFWwindow* window = glfwGetCurrentContext();

    m_totalTime += m_deltaTime;
    m_frames++;
    if (m_totalTime >= 1.0f)
    {
        if (m_frames > highFPS) highFPS = m_frames;
        if (m_frames < lowFPS) lowFPS = m_frames;
        countedFPSes++;
        totalFPS += m_frames;

        std::string title = "Honeycrisp - FPS: " + fmt::to_string(m_frames);
        glfwSetWindowTitle(window, title.c_str());
        m_frames = 0;
        m_totalTime = 0.0f;
    }
}

GameLoop::~GameLoop()
{
    FileSystem::Path data_report_path(fmt::format("build/{}/src/data_report", HNCRSP_BUILD_TYPE));
    std::filesystem::create_directory(data_report_path.string());
    std::ofstream report_file(
        fmt::format("{}/fps_report.txt", data_report_path.string()), std::ios::out | std::ios::app);

    auto time_point = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(time_point);
    char* date = strtok(std::ctime(&time), "\n");

    report_file << fmt::format("[{}, {}] Low - High - Mean FPS: {} - {} - {}\n",
        HNCRSP_GIT_COMMIT_ID,
        date,
        lowFPS,
        highFPS,
        static_cast<float>(totalFPS) / static_cast<float>(countedFPSes)
    );
    report_file.close();
}

HNCRSP_NAMESPACE_END