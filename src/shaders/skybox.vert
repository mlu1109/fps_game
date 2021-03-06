#version 130
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) in vec3 inPosition;

layout(location = 1) uniform mat4 worldView;
layout(location = 2) uniform mat4 viewScreen;

out vec3 texCoord;

void main(void)
{
    texCoord = inPosition;
    gl_Position = viewScreen * worldView * vec4(inPosition, 1.0);
}
