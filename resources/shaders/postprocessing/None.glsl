#version 460 core
out vec4 FragColor;

precision mediump float;

in VS_OUT {
    vec2 TexCoord;
} fs_in;

uniform sampler2D u_framebuffer_color_texture;

void main()
{
    FragColor = texture(u_framebuffer_color_texture, fs_in.TexCoord);
}