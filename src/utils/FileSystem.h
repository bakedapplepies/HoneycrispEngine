#pragma once

#include <filesystem>
#include "utils.h"


HNCRSP_NAMESPACE_START

namespace FileSystem
{
    class Path
    {
    private:
        std::filesystem::path m_path;

    public:
        Path(std::string_view path);
        [[nodiscard]] std::string string() const;
        [[nodiscard]] std::string relative_string() const;
        [[nodiscard]] std::string extension() const;
        void remove_filename();

        bool operator==(const FileSystem::Path& other);
    };
}

HNCRSP_NAMESPACE_END