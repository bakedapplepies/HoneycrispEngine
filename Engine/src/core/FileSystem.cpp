#include "api/core/FileSystem.h"

HNCRSP_NAMESPACE_START

std::filesystem::path FileSystem::Path::s_projectDir(HNCRSP_PROJECT_DIR);

FileSystem::Path::Path(std::string_view path)
{
    m_path = HNCRSP_PROJECT_DIR;
    m_path /= path;
    m_path = m_path.make_preferred();
    m_path = m_path.lexically_normal();
}

std::string FileSystem::Path::file_name() const
{
    return m_path.filename().string();
}

std::string FileSystem::Path::string() const
{
    return m_path.string();
}

std::string FileSystem::Path::relative_string() const
{
    return std::filesystem::relative(m_path, HNCRSP_PROJECT_DIR).string();
}

std::string FileSystem::Path::engine_relative_string() const
{
    static std::filesystem::path enginePath = s_projectDir / "Engine";
    return std::filesystem::relative(m_path, enginePath).string();
}

std::string FileSystem::Path::extension() const
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

void FileSystem::Path::operator=(const char* str)
{
    *this = Path(str);
}

HNCRSP_NAMESPACE_END