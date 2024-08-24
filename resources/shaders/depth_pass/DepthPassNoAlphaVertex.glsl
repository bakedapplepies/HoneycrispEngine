#version 460 core

#define POSITION_LOCATION 0
#define COLOR_LOCATION 1
#define UV_LOCATION 2
#define NORMAL_LOCATION 3

layout(location = POSITION_LOCATION) in vec3 aPos;
layout(location = UV_LOCATION)       in vec2 aTexCoord;

out VS_OUT {
    vec2 TexCoord;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_projection;
    float u_time;
};

uniform mat4 u_model;

void main()
{
    vs_out.TexCoord = aTexCoord;
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}