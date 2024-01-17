#pragma once

#include "src/pch/pch.h"
#include "Debug.h"


#define HNCRSP_STRINGIFY(x) #x

namespace FileSystem
{
    class Path
    {
    private:
        std::filesystem::path m_path;

    public:
        Path(std::string_view path);
        std::string getPath() const
        {
            std::string str = m_path.string();
            return str;
        }
    };
}