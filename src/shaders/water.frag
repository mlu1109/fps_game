#version 130
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable


in vec3 reflectedView; 
in vec3 refractedView;
in vec3 position;

out vec4 outColor;

layout(location = 4) uniform samplerCube texUnit;
layout(location = 7) uniform float time;

void main()
{
    float reflection = 1.0;
    float refraction = 0.4;
    outColor = texture(texUnit, normalize(reflectedView)) * reflection + texture(texUnit, normalize(refractedView)) * refraction;    
    outColor.g *= 1.1;
    outColor *= clamp(position.y, 0.98, 1);
    outColor.a = 0.8;
}
