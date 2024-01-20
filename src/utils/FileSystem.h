#pragma once

#include <filesystem>


HNCRSP_NAMESPACE_START

namespace FileSystem
{
    class Path
    {
    private:
        std::filesystem::path m_path;

    public:
        Path(std::string_view path);
        std::string string() const
        {
            std::string str = m_path.string();
            return str;
        }
    };
}

HNCRSP_NAMESPACE_END