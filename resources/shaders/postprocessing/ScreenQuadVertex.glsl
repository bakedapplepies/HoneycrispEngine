#version 460 core

#define POSITION_LOCATION 0
#define UV_LOCATION 2

layout(location = POSITION_LOCATION) in vec3 aPos;
layout(location = UV_LOCATION) in vec2 aTexCoord;

// Export Vertex Attributes to next shader stage
out VS_OUT {
    vec2 UV;
} vs_out;

// Global uniforms
layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_projection;
    float u_time;
};

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);

    vs_out.UV = aTexCoord;
}