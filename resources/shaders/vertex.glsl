#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

out vec3 VertColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_projection;
    float u_time;
};

uniform mat4 u_model;
uniform mat3 u_normalMatrix;

void main() {
    vec4 worldPos = u_model * vec4(aPos, 1.0);
    worldPos += vec4(0, sin(dot(normalize(vec3(1, 0, 1)), vec3(worldPos.x, 0, worldPos.z))/5 + u_time*2), 0, 0);
    gl_Position = u_projection * u_view * worldPos;
    FragPos = vec3(worldPos);
    Normal = u_normalMatrix * aNormal;
    // Normal = normalize(Normal);
    VertColor = aColor;
    TexCoord = aTexCoord;
}