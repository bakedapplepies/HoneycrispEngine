#version 460 core

#define POSITION_LOCATION 0

layout (location = POSITION_LOCATION) in vec3 aPos;

uniform mat4 u_depthSpaceMatrix;
uniform mat4 u_model;

void main()
{
    gl_Position = u_depthSpaceMatrix * u_model * vec4(aPos, 1.0);
}