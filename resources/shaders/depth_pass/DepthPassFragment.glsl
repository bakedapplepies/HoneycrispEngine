#version 460 core

void main()
{
    gl_FragDepth = gl_FragCoord.z;
    // This is what is done behind the scenes by OpenGL.
}