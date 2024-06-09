#version 460 core
out vec4 FragColor;


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

float luminance(vec3 v)
{
    return dot(v, vec3(0.2126, 0.7152, 0.0722));
}

vec3 change_luminance(vec3 color, float l_out)
{
    float l_in = luminance(color);
    return color * (l_out / l_in);
}

vec3 extended_reinhard_tmo(vec3 color, float max_white)
{
    float l_old = luminance(color);
    float numerator = l_old * (1.0 + (l_old / pow(max_white, 2)));
    float l_new = numerator / (1.0 + l_old);
    return change_luminance(color, l_new);
}

void main()
{
    vec3 color;
    color = vec3(texture(u_framebuffer_color_texture, fs_in.UV));

    // Extended Reinhard Tone mapping
    color = extended_reinhard_tmo(color, 800.0);

    // Gamma Correction
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}