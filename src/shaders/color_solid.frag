#version 130
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location: enable

out vec4 outColor;

layout(location = 5) uniform vec3 color;

void main(void)
{
    outColor = vec4(color, 0.2);
}