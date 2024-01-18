#pragma once

#include "src/pch/pch.h"
#include "Debug.h"


#define HNCRSP_NAMESPACE_START namespace Honeycrisp {
#define HNCRSP_NAMESPACE_END }
#define HNCRSP_STRINGIFY(x) #x

HNCRSP_NAMESPACE_START
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
HNCRSP_NAMESPACE_END