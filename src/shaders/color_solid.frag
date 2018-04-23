#version 330
#extension GL_ARB_explicit_uniform_location : enable

out vec4 outColor;

layout(location = 5) uniform vec4 color;

void main(void)
{
    outColor = color;
}