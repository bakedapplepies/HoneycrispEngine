#pragma once

#include "pch/pch.h"
#include "Object.h"

enum EObjectRenderType
{
    STATIC,
    DYNAMIC
};

class Scene
{
protected:
    std::vector<std::shared_ptr<Object>> objectPtrs;
    // glm::vec3 bgColor;
    
protected:
    template <typename T>
    std::shared_ptr<T> CreateObject(T&& obj, EObjectRenderType render_type)
    {
        static_assert(std::is_same<T, Object>::value == false,
            fmt::format("Type {} is not derived from the Object base class.")
        );

        std::shared_ptr<T> temp_ptr = std::make_shared<T>(obj);
        objectPtrs.push_back(temp_ptr);
        return temp_ptr;
    }
};