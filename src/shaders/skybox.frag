#version 130
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

in vec3 texCoord;
out vec4 outColor;

layout(location = 12) uniform samplerCube texUnit;

void main()
{
    outColor = texture(texUnit, texCoord);
}