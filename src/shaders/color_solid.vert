#version 130
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location: enable


layout(location = 0) in vec3 inPosition;

layout(location = 0) uniform mat4 modelWorld;
layout(location = 1) uniform mat4 worldView;
layout(location = 2) uniform mat4 viewScreen;

void main(void)
{
    mat4 mvp = viewScreen * worldView * modelWorld;
    gl_Position = mvp * vec4(inPosition, 1.0);
}
