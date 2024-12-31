#include "FileSystem.h"


HNCRSP_NAMESPACE_START

FileSystem::Path::Path(std::string_view path)
{
    m_path = std::filesystem::current_path() / "../../../";
    m_path /= path;
    m_path = m_path.make_preferred();
    m_path = m_path.lexically_normal();
}

HNCRSP_NODISCARD std::string FileSystem::Path::string() const
{
    return m_path.string();
}

HNCRSP_NODISCARD std::string FileSystem::Path::relative_string() const
{
    return std::filesystem::relative(m_path, HNCRSP_PROJECT_DIR).string();
}

HNCRSP_NODISCARD std::string FileSystem::Path::extension() const
{
    return m_path.extension().string();
}

void FileSystem::Path::remove_filename()
{
    m_path.remove_filename();
}

bool FileSystem::Path::operator==(const FileSystem::Path& other)
{
    return this->string() == other.string();
}

HNCRSP_NAMESPACE_END