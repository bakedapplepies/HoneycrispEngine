#include "utils.h"


std::string FileSystem::Path(const std::string& path, const std::source_location& location)
{
    std::filesystem::path absPath("../../");
    absPath /= location.file_name();
    absPath = absPath.remove_filename();
    absPath /= path;
    // absPath = std::filesystem::absolute(absPath);
    absPath = absPath.make_preferred();
    absPath = absPath.lexically_normal();
    return absPath.string();
}