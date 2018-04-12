#version 330
#extension GL_ARB_explicit_uniform_location : enable
layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 normal;
//layout(location = 2) in vec4 color;
//layout(location = 3) in vec2 texCoord;

layout(location = 0) uniform mat4 modelWorld;
layout(location = 1) uniform mat4 worldView;
layout(location = 2) uniform mat4 viewScreen;
layout(location = 3) uniform mat4 normal;

void main(void)
{
    gl_Position = viewScreen * worldView * modelWorld * vec4(position, 1.0);
    texCoord = inTexCoord;
}
