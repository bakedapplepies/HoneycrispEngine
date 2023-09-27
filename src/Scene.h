#pragma once

#include "pch/pch.h"
#include "Debug.h"
#include "Object.h"
#include "types/types_index.h"

enum EObjectRenderType
{
    NONE,
    STATIC,
    DYNAMIC
};

class Scene
{
protected:
    std::vector< std::shared_ptr<Object> > m_staticObjectPtrs;
    std::vector< std::shared_ptr<Object> > m_dynamicObjectPtrs;
    std::vector< std::shared_ptr<Object> > m_nonRenderObjectPtrs;
    // static Scene* m_sceneInstance;
    // glm::vec3 bgColor;
    
public: // protected
    template <typename T>
    SceneObject<T> CreateObject(T&& obj, EObjectRenderType render_type)
    {
        Object test_obj = static_cast<Object>(obj);
        
        std::shared_ptr<T> temp_ptr = std::make_shared<T>(std::move(obj));

        switch (render_type)
        {
            case EObjectRenderType::NONE:
                m_nonRenderObjectPtrs.push_back(temp_ptr);
                break;
            case EObjectRenderType::STATIC:
                m_staticObjectPtrs.push_back(temp_ptr);
                break;
            case EObjectRenderType::DYNAMIC:
                m_dynamicObjectPtrs.push_back(temp_ptr);
                break;
            default:
                Debug::Error("Check object type (EObjectRenderType enum not valid.)");
                break;
        }

        return temp_ptr;
    }

    virtual void OnUpdate(Shader& shader) = 0;
};