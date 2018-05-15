#version 130
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable


in vec3 reflectedView; 
in vec3 refractedView;
in vec3 position;
in vec3 normal;

out vec4 outColor;

layout(location = 0) uniform mat4 modelWorld;
layout(location = 1) uniform mat4 worldView;

layout(location = 4) uniform vec3 lightPosition;
layout(location = 5) uniform vec3 lightColor;
layout(location = 6) uniform vec3 cameraPosition;

layout(location = 7) uniform vec3 materialAmbient;
layout(location = 8) uniform vec3 materialDiffuse;
layout(location = 9) uniform vec3 materialSpecular;
layout(location = 10) uniform float materialShine;

layout(location = 11) uniform float time;
layout(location = 12) uniform samplerCube texUnit;

void main()
{
    float a = materialShine;

    // Point light
    //vec3 s = normalize(vec3(worldView * vec4(lightPosition, 1.0)) - position);
    // Directional light
    vec3 s = normalize(vec3(worldView * vec4(lightPosition, 0.0)));
    vec3 ambient = vec3(0, 0, 0);
    vec3 diffuse = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);

    // Ambient: i_amb = k_a * i_a
    ambient += materialAmbient * lightColor;
    
    vec3 n = normalize(normal);
    // Diffuse: i_diff = k_d * i_s * cos(theta)
    float cos_theta = max(dot(s, n), 0);
    diffuse += materialDiffuse * lightColor * cos_theta;

    vec3 lightResult = ambient + diffuse;
    
    // Skybox Reflection/Refraction
    float reflection = 0.8;
    float refraction = 0.2;
    outColor = texture(texUnit, normalize(reflectedView)) * reflection + texture(texUnit, normalize(refractedView)) * refraction + vec4(lightResult, 0.3);    
    outColor.g *= 1.1;
    outColor *= clamp(position.y, 0.98, 1);
    outColor.a = 0.75;
}
