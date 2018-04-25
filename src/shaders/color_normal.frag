#version 130
#extension GL_ARB_explicit_uniform_location: enable
#extension GL_ARB_explicit_attrib_location: enable

in vec3 normal;
out vec4 outColor;

void main(void)
{
    outColor = vec4(normal, 1.0);
}