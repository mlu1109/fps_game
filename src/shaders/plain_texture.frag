#version 130
#extension GL_ARB_explicit_uniform_location: enable
#extension GL_ARB_explicit_attrib_location: enable

in vec2 texCoord;
out vec4 outColor;

layout(location = 4) uniform sampler2D texUnit;

void main(void)
{
    outColor = texture(texUnit, texCoord);
}