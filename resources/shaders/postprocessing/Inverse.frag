#version 450 core
out vec4 FragColor;



in VS_OUT {
    vec2 UV;
} fs_in;

uniform sampler2D u_framebuffer_color_texture;
uniform vec2 u_resolution;

void main()
{
    vec3 color = vec3(texture(u_framebuffer_color_texture, fs_in.UV));
    color = vec3(1.0) - color;
    FragColor = vec4(color, 1.0);
}