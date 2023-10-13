#pragma once

#include "pch/pch.h"
#include "Debug.h"
#include "Object.h"
#include "Mesh.h"
#include "NonRenderable.h"
#include "types/types_index.h"

enum EObjectRenderType
{
    NONE,
    STATIC,
    DYNAMIC
};

struct RenderableShaderGroupInfo
{
    std::vector< std::shared_ptr<Renderable> >objectShaderGroup;
    std::shared_ptr<Shader> shader;
};

class Scene
{
private:
    static std::shared_ptr<Shader> basicShader;

protected:
    std::unordered_map<GLuint, RenderableShaderGroupInfo> m_renderObjectPtrs;
    std::vector< std::shared_ptr<NonRenderable> > m_nonRenderObjectPtrs;

public:
    glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);

protected:
    template <typename T>
    SceneObject<T> CreateObject(std::true_type, T&& obj, EObjectRenderType render_type, std::shared_ptr<Shader> shader)
    {
        if (render_type == EObjectRenderType::STATIC)
        {
            SceneObject<T> objPtr = std::make_shared<T>(std::move(obj));
            // TODO: batching
            if (shader)
            {
                m_renderObjectPtrs[shader->getID()].objectShaderGroup.push_back(objPtr);
                if (!m_renderObjectPtrs[shader->getID()].shader)
                {
                    m_renderObjectPtrs[shader->getID()].shader = shader;
                }
            }
            else
            {
                m_renderObjectPtrs[basicShader->getID()].objectShaderGroup.push_back(objPtr);
                if (!m_renderObjectPtrs[basicShader->getID()].shader)
                {
                    m_renderObjectPtrs[basicShader->getID()].shader = basicShader;
                }
            }
            return objPtr;
        }
        else if (render_type == EObjectRenderType::DYNAMIC)
        {
            SceneObject<T> objPtr = std::make_shared<T>(std::move(obj));
            // TODO: instancing
            if (shader)
            {
                m_renderObjectPtrs[shader->getID()].objectShaderGroup.push_back(objPtr);
                if (!m_renderObjectPtrs[shader->getID()].shader)
                {
                    m_renderObjectPtrs[shader->getID()].shader = shader;
                }
            }
            else
            {
                m_renderObjectPtrs[basicShader->getID()].objectShaderGroup.push_back(objPtr);
                if (!m_renderObjectPtrs[basicShader->getID()].shader)
                {
                    m_renderObjectPtrs[basicShader->getID()].shader = basicShader;
                }
            }
            return objPtr;
        }
        else
        {
            // Debug::Error("EObjectRenderType enum must be either STATIC or DYNAMIC.");
            assert(false && "EObjectRenderType enum must be either STATIC or DYNAMIC.");
        }
    }

    template <typename T>
    SceneObject<T> CreateObject(std::false_type, T&& obj, EObjectRenderType render_type, std::shared_ptr<Shader> shader)
    {
        SceneObject<T> objPtr = std::make_shared<T>(std::move(obj));
        m_nonRenderObjectPtrs.push_back(objPtr);
        return objPtr;
    }

    template <typename T>
    SceneObject<T> CreateObject(T&& obj, EObjectRenderType render_type = EObjectRenderType::NONE, std::shared_ptr<Shader> shader = nullptr)
    {
        if (!basicShader)
        {
            basicShader = std::make_shared<Shader>(
                std::ifstream("../resources/shaders/defaultvertex.glsl"),
                std::ifstream("../resources/shaders/defaultfragment.glsl")
            );
        }
        
        return CreateObject(std::is_base_of<Renderable, T>(), std::move(obj), render_type, shader);
    }

    void Draw(void) const
    {
        for (auto iter = m_renderObjectPtrs.begin(); iter != m_renderObjectPtrs.end(); iter++)
        {
            iter->second.shader->Use();
            for (SceneObject<Renderable> obj : iter->second.objectShaderGroup)
            {
                obj->Draw(iter->second.shader);
            }
        }
    }

public:
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