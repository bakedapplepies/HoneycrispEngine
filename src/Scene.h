#pragma once

#include "pch/pch.h"
#include "Debug.h"
#include "Object.h"
#include "Mesh.h"
#include "types/types_index.h"

enum EObjectRenderType
{
    NONE,
    STATIC,
    DYNAMIC
};


class Scene
{
private:
    static std::shared_ptr<Shader> basicShader;

protected:
    std::unordered_map< std::shared_ptr<Shader>, std::vector< std::shared_ptr<Mesh> > > m_renderObjectPtrs;
    std::vector< std::shared_ptr<Object> > m_nonRenderObjectPtrs;

public:
    glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);

public: // protected
    template <typename T>
    SceneObject<T> CreateObject(T&& obj, EObjectRenderType render_type, std::shared_ptr<Shader> shader = nullptr)
    {
        Object* test_obj = static_cast<Object*>(&obj);
        assert(test_obj != nullptr);  // TODO: Actually test with an invalid call
        if (!basicShader)
        {
            basicShader = std::make_shared<Shader>(
                std::ifstream("../resources/shaders/defaultvertex.glsl"),
                std::ifstream("../resources/shaders/defaultfragment.glsl")
            );
        }
        
        SceneObject<T> temp_ptr = std::make_shared<T>(std::move(obj));
        switch (render_type)
        {
            case EObjectRenderType::NONE:
                m_nonRenderObjectPtrs.push_back(temp_ptr);
                break;
            case EObjectRenderType::STATIC:
                // TODO: batching
                if (shader)
                    m_renderObjectPtrs[shader].push_back(temp_ptr);
                else
                    m_renderObjectPtrs[basicShader].push_back(temp_ptr);
                break;
            case EObjectRenderType::DYNAMIC:
                // TODO: instancing
                if (shader)
                    m_renderObjectPtrs[shader].push_back(temp_ptr);
                else
                    m_renderObjectPtrs[basicShader].push_back(temp_ptr);
                break;
            default:
                Debug::Error("Check object type (EObjectRenderType enum not valid.)");
                assert(false);
                break;
        }

        return temp_ptr;
    }

    void Draw(void) const
    {
        for (auto iter = m_renderObjectPtrs.begin(); iter != m_renderObjectPtrs.end(); iter++)
        {
            iter->first->Use();
            for (SceneObject<Mesh> obj : iter->second)
            {
                obj->Draw(*(iter->first));
            }
        }
    }

    virtual void OnUpdate() = 0;
};

/*  Binary search-insert
void add_val(std::vector<Object>& vec, int v)
{
    unsigned int l = 0, r = vec.size() - 1;
    if (r == -1) vec.push_back({v, "abc"});
    else if (v < vec[l].id) vec.insert(vec.begin() + 0, {v, "abc"});
    else if (v > vec[r].id) vec.push_back({v, "abc"});
    else
    {
        unsigned int m = (l + r) / 2;
        while (vec[m].id != v)
        {
            if (v < vec[m].id)
            {
                r = m - 1;
            }
            else if (vec[m].id < v)
            {
                l = m + 1;
            }
            m = (l + r) / 2;
        }
        vec.insert(vec.begin() + m, {v, "abc"});
    }
}
*/