#version 460 core
out vec4 FragColor;

precision mediump float;

in VS_OUT {
    vec2 UV;
} fs_in;

// Global uniforms
layout (std140, binding = 0) uniform Matrices
{
    mat4 u_view;
    mat4 u_projection;
    float u_time;
};

uniform sampler2D u_framebuffer_color_texture;

void main()
{
    vec3 color;

    color = vec3(texture(u_framebuffer_color_texture, fs_in.UV));
    // vec2 uv = fs_in.UV;

    // if (uv.x < 0.5)
    // {
    //     color = vec3(0.5, 0.0, 0.0);
    // }
    // else
    // {
    //     color = vec3(1.0, 0.0, 0.0);
    // }

    // // color = uv.x;

    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color, 1.0);
}