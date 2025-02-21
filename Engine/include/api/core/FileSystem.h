#pragma once

#include <filesystem>
#include <string>
#include "api/core/Defines.h"

HNCRSP_NAMESPACE_START

namespace FileSystem
{
    class Path
    {
    private:
        static std::filesystem::path s_projectDir;
        std::filesystem::path m_path;

    public:
        Path(std::string_view path);
        HNCRSP_NODISCARD std::string file_name() const;
        HNCRSP_NODISCARD std::string string() const;
        HNCRSP_NODISCARD std::string relative_string() const;
        HNCRSP_NODISCARD std::string extension() const;
        HNCRSP_NODISCARD std::string engine_relative_string() const;       
        void remove_filename();

        bool operator==(const FileSystem::Path& other);
        void operator=(const char* str);
    };
}

HNCRSP_NAMESPACE_END