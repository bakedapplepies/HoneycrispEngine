#version 400 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

// send to fragment shader
out vec3 outColor;
out vec2 TexCoord;

// uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    outColor = aColor;
    TexCoord = aTexCoord;
}