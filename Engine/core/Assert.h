#pragma once

#include <fmt/format.h>

// Taken from https://github.com/travisvroman/kohi/blob/main/kohi.core/src/debug/kassert.h

// Try via __has_builtin first.
#if defined(__has_builtin) && !defined(__ibmxl__)
#    if __has_builtin(__builtin_debugtrap)
#        define HNCRSP_DEBUG_BREAK() __builtin_debugtrap()
#    elif __has_builtin(__debugbreak)
#        define HNCRSP_DEBUG_BREAK() __debugbreak()
#    endif
#endif

// If not setup, try the old way.
#if !defined(HNCRSP_DEBUG_BREAK)
#    if defined(__clang__) || defined(__gcc__)
#        define HNCRSP_DEBUG_BREAK() __builtin_trap()
#    elif defined(_MSC_VER)
#        include <intrin.h>
#        define HNCRSP_DEBUG_BREAK() __debugbreak()
#    else
// Fall back to x86/x86_64
#        define HNCRSP_DEBUG_BREAK() asm { int 3 }
#    endif
#endif

#ifndef HNCRSP_RELEASE
#   define HNCRSP_ASSERT(expr, msg)                                                      \
        {                                                                                \
            if (expr) {}                                                                 \
            else {                                                                       \
                fmt::println("[ASSERTION FAILED | {}:{}] {}", __FILE__, __LINE__, msg);  \
                HNCRSP_DEBUG_BREAK();                                                    \
            }                                                                            \
        }                                   
#else
#   ifdef HNCRSP_RELEASE_LOGGING  // TODO: Make this an option in CMake 
#       define HNCRSP_ASSERT(expr, msg) fmt::println("ERROR: {}", msg);
#   else
#       define HNCRSP_ASSERT(expr, msg)
#   endif
#endif