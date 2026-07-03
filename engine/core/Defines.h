#pragma once

#define HNCRSP_NAMESPACE_START namespace Honeycrisp {
#define HNCRSP_NAMESPACE_END }
#if __cplusplus >= 201703L
    #define HNCRSP_NODISCARD [[nodiscard]]
#else
    #define HNCRSP_NODISCARD
#endif