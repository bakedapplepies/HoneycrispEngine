#version 400 core
out vec4 FragColor;

in vec3 outColor;
in vec2 TexCoord;

// uniforms
uniform float time;

// textures
uniform sampler2D uTexture0;

void main()
{
    // FragColor = vec4(outColor, 1.0);
    FragColor = texture(uTexture0, TexCoord);
}