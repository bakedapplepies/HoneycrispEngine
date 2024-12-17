#version 460 core

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

out VS_OUT_WAVE_DATA {
    float AngularVelocity;
    float InitialPhase;
} vs_out_wave_data;

layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_ortho;
    mat4 u_projection;
    float u_time;
};

uniform mat4 u_model;
uniform mat3 u_normalMatrix;

void main() {
    vec4 worldPos = u_model * vec4(aPos, 1.0);
    float angularVelocity = dot(normalize(vec3(1, 0, 1)), vec3(worldPos.x, 0, worldPos.z))/5;
    float initialPhase = u_time*2;
    vs_out_wave_data.AngularVelocity = angularVelocity;
    vs_out_wave_data.InitialPhase = initialPhase;
    worldPos += vec4(0, sin(angularVelocity + initialPhase), 0, 0);
    
    vs_out.FragPos = vec3(worldPos);
    vs_out.Normal = u_normalMatrix * aNormal;
    vs_out.VertColor = aColor;
    vs_out.TexCoord = aTexCoord;

    gl_Position = u_projection * u_view * worldPos;
}