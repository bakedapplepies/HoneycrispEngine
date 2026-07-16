#include <Envy/EnvyInstance.h>
#include "editor/AppWindow.h"

#include <fmt/format.h>

int main(int argc, char** argv)
{
    // Determine window size
    if (argc != 3)
    {
        fmt::println("Honeycrisp TestClient: Invalid number of arguments (Must be 2.)");
        return EXIT_FAILURE;
    }
    
    char* windowWidthStr = argv[1];
    char* windowHeightStr = argv[2];

    int windowWidth = std::stoi(windowWidthStr);
    int windowHeight = std::stoi(windowHeightStr);

    // Object creation
    Honeycrisp::WindowConfig windowConfig {
        .name = "HoneycrispEngine",
        .width = windowWidth,
        .height = windowHeight
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
    
    return EXIT_SUCCESS;
}