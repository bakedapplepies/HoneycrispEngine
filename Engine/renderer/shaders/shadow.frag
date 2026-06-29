#version 460 core

// Data from Vertex stage
layout (location = 0) in V_OUT
{
    vec3 o_WorldPosition;
    flat uint instanceID;
} v_out;

void main()
{
    // gl_FragDepth = gl_FragCoord.z;
}   