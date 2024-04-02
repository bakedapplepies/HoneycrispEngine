#pragma once

#include "src/utils/FileSystem.h"


#define HNCRSP_TERMINATE(msg) Honeycrisp::terminate(msg, FileSystem::Path(__FILE__), __LINE__);

namespace Honeycrisp
{
    void terminate(const char* msg, const FileSystem::Path& file, unsigned int line);
}