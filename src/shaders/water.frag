#version 130
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable
#extension GL_ARB_gpu_shader5 : enable


in vec3 reflectedView; 
in vec3 refractedView;

out vec4 outColor;

layout(location = 4) uniform samplerCube texUnit;

void main()
{
    float reflection = 1.0;
    float refraction = 0.4;
    outColor = texture(texUnit, normalize(reflectedView)) * reflection + texture(texUnit, normalize(refractedView)) * refraction;    
    outColor.a = 0.9;
}