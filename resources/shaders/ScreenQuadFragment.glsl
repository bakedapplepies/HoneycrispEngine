#version 460 core
out vec4 FragColor;

precision mediump float;

in VS_OUT {
    vec2 TexCoord;
} fs_in;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, fs_in.TexCoord);
}