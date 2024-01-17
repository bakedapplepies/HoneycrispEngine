#pragma once

#include "src/pch/pch.h"
#include "Debug.h"


namespace JsonUtil
{
    using nlohmann::json;

    json parseFile(const std::string& path, const std::source_location& location = std::source_location::current())
    {
        HNCRSP_LOG_INFO(location.file_name());

        return {};
    }
}