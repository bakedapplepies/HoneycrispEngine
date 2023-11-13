#pragma once

#include "../pch/pch.h"

namespace FileSystem
{
    std::string Path(const std::string& path, const std::source_location& location = std::source_location::current());
}