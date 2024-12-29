#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 VertColor;
} fs_in;

void main()
{
    FragColor = vec4(fs_in.VertColor, 1.0);
}