#version 450 core

#define POSITION_LOCATION 0
#define COLOR_LOCATION 1
#define UV_LOCATION 2
#define NORMAL_LOCATION 3

layout(location = POSITION_LOCATION) in vec3 aPos;
layout(location = COLOR_LOCATION)    in vec3 aColor;
layout(location = UV_LOCATION)       in vec2 aTexCoord;
layout(location = NORMAL_LOCATION)   in vec3 aNormal;

out VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} vs_out;

uniform mat4 u_model;
uniform mat3 u_normalMatrix;
layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_ortho;
    mat4 u_projection;
    float u_time;
};

void main()
{
    vs_out.VertColor = aColor;
    vs_out.TexCoord = aTexCoord;
    vs_out.Normal = mat3(transpose(inverse(u_view * u_model))) * aNormal;
    vec4 worldPos = u_model * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(worldPos);

    gl_Position = u_view * worldPos;
}