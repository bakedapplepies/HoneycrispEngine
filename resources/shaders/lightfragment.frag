#version 400 core
out vec4 FragColor;

in vec3 vertColor;

uniform vec3 uColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}