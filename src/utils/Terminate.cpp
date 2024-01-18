#include "Terminate.h"


namespace Honeycrisp
{
    void terminate(const char* msg, const char* file, unsigned int line)
    {
        glfwTerminate();
        std::cout << fmt::format("[TERMINATE at {}, Ln: {}]: {}\n", file, line, msg);
        assert(false);
    }
}