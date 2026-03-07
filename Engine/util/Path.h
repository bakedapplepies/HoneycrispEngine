#pragma once

#include <string_view>

class Path
{
public:
    Path(std::string_view path);
    ~Path() = default;

    std::string Str() const;

private:
    std::string_view m_path;
};
