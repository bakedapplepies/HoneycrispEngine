#pragma once

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
#   define HNCRSP_ASSERT(expr)              \
        {                                   \
            if (expr) {}                    \
            else { HNCRSP_DEBUG_BREAK(); }  \
        }                                   
#else
#   define HNCRSP_ASSERT(expr)
#endif