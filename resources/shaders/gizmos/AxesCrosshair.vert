#version 450 core

#define POSITION_LOCATION 0
#define COLOR_LOCATION 1

layout(location = POSITION_LOCATION) in vec3 aPos;
layout(location = COLOR_LOCATION)    in vec3 aColor;

// Export Vertex Attributes to next shader stage
out VS_OUT {
    vec3 VertColor;
} vs_out;

void main()
{
    vs_out.VertColor = aColor;

    gl_Position = vec4(aPos, 1.0);
}