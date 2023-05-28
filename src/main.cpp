// #define FMT_HEADER_ONLY

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