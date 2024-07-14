#include "src/pch/hncrsp_pch.h"


namespace Honeycrisp
{
    void terminate(const char* msg, const FileSystem::Path& file, unsigned int line)
    {
        glfwTerminate();
        std::cout << fmt::format("[TERMINATE at ~{}, Ln: {}]: {}\n", file.relative_string(), line, msg);
        assert(false);
    }
}