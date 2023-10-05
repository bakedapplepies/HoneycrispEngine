#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

out vec3 VertColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_model;
uniform mat3 u_normalMatrix;
layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_projection;
    float u_time;
};

void main()
{
    VertColor = aColor;
    TexCoord = aTexCoord;
    Normal = u_normalMatrix * aNormal;
    vec4 worldPos = u_model * vec4(aPos, 1.0);
    FragPos = vec3(worldPos);
    gl_Position = u_projection * u_view * worldPos;
}