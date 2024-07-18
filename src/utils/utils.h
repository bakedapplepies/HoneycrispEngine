#pragma once

#define HNCRSP_NAMESPACE_START namespace Honeycrisp {
#define HNCRSP_NAMESPACE_END }
#define HNCRSP_STRINGIFY(x) #x
#define HNCRSP_CONST_AUTO_REF const auto&

#if __cplusplus >= 201703L
    #define HNCRSP_NODISCARD [[nodiscard]]
#else
    #define HNCRSP_NODISCARD
#endif

#include <string>

namespace Honeycrisp
{
    std::string exec(const char* cmd);
}