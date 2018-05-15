#version 130
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) uniform mat4 modelWorld;
layout(location = 1) uniform mat4 worldView;
layout(location = 2) uniform mat4 viewScreen;

out vec2 texCoord;

void main(void)
{
    texCoord = inTexCoord;
    gl_Position = viewScreen * worldView * modelWorld * vec4(inPosition, 1.0);
}
