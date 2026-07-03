#pragma once

#include <Envy/EnvyInstance.h>
#include <Envy/opengl/ShaderProgram.h>
#include <Envy/opengl/Texture2D.h>

#include "core/Defines.h"
#include "core/Types.h"

HNCRSP_NAMESPACE_START

class PostprocessQueue
{
public:
    PostprocessQueue(const Envy::EnvyInstance* envy_instance, int width, int height);
    ~PostprocessQueue();

    void AddCompute(const Envy::ShaderProgram* compute_shader);

    // May invalidate the input. Do not attempt to use the input
    // within the same frame.
    const GLResource<Envy::Texture2D> Process(const GLResource<Envy::Texture2D>& input,
                                              int group_x, int group_y, int group_z) const;

private:
    std::vector<const Envy::ShaderProgram*> m_computeShaders;
    GLResource<Envy::Texture2D> m_buffer = GLResource<Envy::Texture2D>::empty;
};

HNCRSP_NAMESPACE_END