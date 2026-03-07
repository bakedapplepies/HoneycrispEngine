#pragma once

#include <memory>

#include <Envy/EnvyInstance.h>

#include "editor/GLFWCallbacks.h"
#include "Renderer.h"

struct WindowConfig
{
    const char* name;
    int width;
    int height;
};

struct GLFWwindow;
class Application
{
public:
    Application();
    ~Application();

    bool Create(const WindowConfig& window_config, const Envy::EnvyInstance& envy_instance);
    void Destroy();
    void Run();

private:
    void _ProcessInput(float delta_time);
    void _LoadShaderPrograms() const;
    void _LoadTexture2Ds() const;

private:
    GLFWwindow* m_window = nullptr;
    std::unique_ptr<GLFWUserData> m_GLFWUserData = nullptr;
    bool m_applicationIsActive = false;

    // This will be where graphics backends can be switched in and out
    const Envy::EnvyInstance* m_envyInstance = nullptr;
    std::unique_ptr<Renderer> m_renderer = nullptr;
    Camera* m_mainCamera = nullptr;
};