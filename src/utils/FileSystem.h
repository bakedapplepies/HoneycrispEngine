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
        [[nodiscard]] std::string string() const;
        [[nodiscard]] std::string relative_string() const;
        void remove_filename();
    };
}

HNCRSP_NAMESPACE_END