#version 460 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_view;
    mat4 u_projection;
    float u_time;
};

// uniform mat4 u_view_NoTrans;

void main()
{
    TexCoords = aPos;  // using local space as uvs
    gl_Position = u_projection * mat4(mat3(u_view)) * vec4(aPos, 1.0);
}