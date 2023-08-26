#include "Window.h"


int main()
{
    {
        Window window;
        window.Loop();
    }
    glfwTerminate();

    return 0;
}