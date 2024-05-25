#version 460 core
out vec4 FragColor;

precision mediump float;

in VS_OUT {
    vec2 UV;
} fs_in;

uniform sampler2D u_framebuffer_color_texture;
uniform vec2 u_resolution;
// uniform float
const float offset = 1.0 / 300.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0,     offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0),   // center-left
        vec2( 0.0,     0.0),   // center-center
        vec2( offset,  0.0),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0,   - offset), // bottom-center
        vec2( offset, -offset)  // bottom-right 
    );

    float kernel[9] = float[](
        0,  0,  0,
        0,  1,  0,
        0,  0,  0
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_framebuffer_color_texture, fs_in.UV + offsets[i]));
    }

    vec3 color = vec3(0.0);
    for (int i = 0; i < 9; i++)
    {
        color += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(color, 1.0);
}