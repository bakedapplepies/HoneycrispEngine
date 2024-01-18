#include "utils.h"


HNCRSP_NAMESPACE_START
FileSystem::Path::Path(std::string_view path)
{
    m_path = std::filesystem::current_path() / "../../";
    m_path /= path;
    m_path = m_path.make_preferred();
    m_path = m_path.lexically_normal();
}
HNCRSP_NAMESPACE_END