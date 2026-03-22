#include <Envy/EnvyInstance.h>
#include "renderer/AppWindow.h"

int main(int argc, char** argv)
{
    // Object creation
    Honeycrisp::WindowConfig windowConfig {
        .name = "HoneycrispEngine",
        .width = 2560,
        .height = 1440
    };
    Honeycrisp::Application window;
    Envy::EnvyInstance envyInstance;
    
    window.Create(windowConfig, envyInstance);
    envyInstance.Init();

    // Execution
    window.Run();

    // Clean-up
    envyInstance.Shutdown();
    window.Destroy();
    
    return 0;
}