#pragma once

// TODO: Don't put this file here

#define HNCRSP_NAMESPACE_START namespace Honeycrisp {
#define HNCRSP_NAMESPACE_END }
#define HNCRSP_STRINGIFY(x) #x
#define HNCRSP_CONST_AUTO_REF const auto&

#if defined(__clang__) || defined (__GNUC__)
#   define HNCRSP_STATIC_ASSERT static_assert
#else
#   define HNCRSP_STATIC_ASSERT static_assert
#endif

#if __cplusplus >= 201703L
    #define HNCRSP_NODISCARD [[nodiscard]]
#else
    #define HNCRSP_NODISCARD
#endif