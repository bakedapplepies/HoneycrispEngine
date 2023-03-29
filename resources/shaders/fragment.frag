#version 400 core
out vec4 FragColor;

in vec3 outColor;
in vec2 TexCoord;

uniform float time;
uniform sampler2D uTexture0;
// uniform sampler2D uTexture2;

void main()
{
    // float newR = outColor.x * sin(time) + 0.1;
    // float newG = outColor.y * sin(time) + 0.3;
    // float newB = outColor.z * sin(time) + 0.5;

    // FragColor = vec4(newR, newG, newB, 1.0);

    // FragColor = vec4(outColor, 1.0);
    FragColor = texture(uTexture0, TexCoord);
}