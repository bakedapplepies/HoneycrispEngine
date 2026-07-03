#version 460 core

// Re-declare built-in variables for stricter interface
// due to more flexible pipelines being able to choose different
// shader programs.
out gl_PerVertex
{
    vec4 gl_Position;
    // float gl_PointSize;
    // float gl_ClipDistance[];
};

layout (location = 0) in vec3 i_Position;
layout (location = 1) in vec3 i_Normal;
layout (location = 2) in vec2 i_UV;
layout (location = 3) in vec3 i_inst_Translation;

layout (binding = 0, std140) uniform GlobalUBO
{
    float u_time;
    mat4 u_view;
    mat4 u_projection;
    vec4 u_cameraPos;
    mat4 u_lightSpace;
};

uniform mat4 u_model;

void main()
{
    vec4 worldPosition = u_model * vec4(i_Position + i_inst_Translation, 1.0);

    gl_Position = u_lightSpace * worldPosition;
}