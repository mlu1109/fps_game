#version 130
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location: enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) uniform mat4 modelWorld;
layout(location = 1) uniform mat4 worldView;
layout(location = 2) uniform mat4 viewScreen;
layout(location = 3) uniform mat4 worldViewNormal;

out vec3 normal;
out vec3 position;

void main(void)
{
    normal = mat3(worldViewNormal) * inNormal;
    position = vec3(worldView * modelWorld * vec4(inPosition, 1.0));
    gl_Position = viewScreen * worldView * modelWorld * vec4(inPosition, 1.0);
}
