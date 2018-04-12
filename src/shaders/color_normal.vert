#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
//layout(location = 2) in vec4 color;
//layout(location = 3) in vec2 texCoord;

layout(location = 0) uniform mat4 modelWorld;
layout(location = 1) uniform mat4 worldView;
layout(location = 2) uniform mat4 viewScreen;

out vec3 normal;

void main(void)
{
    normal = inNormal;
    mat4 mvp = viewScreen * worldView * modelWorld;
    gl_Position = mvp * vec4(inPosition, 1.0);
}
