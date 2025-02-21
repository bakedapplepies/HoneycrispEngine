#version 450 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_view;
    mat4 u_ortho;
    mat4 u_projection;
    float u_time;
};

void main()
{
    TexCoords = vec3(aPos.x, aPos.y, -aPos.z);  // using local space as uvs
    vec4 pos = u_projection * mat4(mat3(u_view)) * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}