#pragma once

#include <iostream>

#define HNCRSP_NAMESPACE_START namespace Honeycrisp {
#define HNCRSP_NAMESPACE_END }
#define HNCRSP_STRINGIFY(x) #x
#define HNCRSP_CONST_AUTO_REF const auto&

HNCRSP_NAMESPACE_START

std::string exec(const char* cmd);

HNCRSP_NAMESPACE_END