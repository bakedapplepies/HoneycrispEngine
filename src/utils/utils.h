#pragma once

#include "../pch/pch.h"

namespace FileSystem
{
    struct Path
    {
        std::string path = "";
        Path(const std::string& path)
        {
            std::filesystem::path absPath("../../");
            absPath /= path;
            
            absPath = absPath.make_preferred();
            absPath = absPath.lexically_normal();
            this->path = absPath.string();
        }
    };
}

namespace Algo
{
}