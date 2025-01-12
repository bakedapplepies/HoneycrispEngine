#pragma once

#include "api/core/Defines.h"
#include "api/core/Assert.h"
#include "api/core/FileSystem.h"


#define HNCRSP_TERMINATE(...) Honeycrisp::__Terminate(FileSystem::Path(__FILE__), __LINE__, __VA_ARGS__);

HNCRSP_NAMESPACE_START

template <typename... Args>
void __Terminate(
    const FileSystem::Path& file,
    unsigned int line,
    fmt::format_string<Args...> format_str,
    Args&&... args
) {
    glfwTerminate();
    fmt::print("[TERMINATE at ~{}, Ln: {}]: {}\n",
        file.relative_string(),
        line,
        fmt::format(format_str, std::forward<Args>(args)...)
    );
    HNCRSP_ASSERT(false);
}
    
HNCRSP_NAMESPACE_END