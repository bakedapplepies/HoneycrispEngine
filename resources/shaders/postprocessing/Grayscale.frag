#version 450 core
out vec4 FragColor;

precision mediump float;

in VS_OUT {
    vec2 UV;
} fs_in;

uniform sampler2D u_framebuffer_color_texture;
uniform vec2 u_resolution;

void main()
{
    vec3 color = vec3(texture(u_framebuffer_color_texture, fs_in.UV));

    vec3 grayscaleVec = vec3(0.299, 0.587, 0.114);
    float grayscale = dot(color, grayscaleVec);

    color = vec3((color.r + color.g + color.b) / 3.0);

    FragColor = pow(vec4(vec3(grayscale), 1.0), vec4(1.0/2.2));
}