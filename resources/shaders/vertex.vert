#version 400 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

out vec3 VertColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normalMatrix;
uniform float u_time;

void main() {
    vec4 localPos = u_model * vec4(aPos, 1.0);
    localPos += vec4(0, sin(dot(vec3(1, 0, 1), vec3(localPos.x, 0, localPos.z)) + u_time*2), 0, 0);
    gl_Position = u_projection * u_view * localPos;
    FragPos = vec3(localPos);
    Normal = u_normalMatrix * aNormal;
    VertColor = aColor;
    TexCoord = aTexCoord;
}