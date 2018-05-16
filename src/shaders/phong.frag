#version 130
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location: enable

in vec3 normal;
in vec3 position;

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

layout(location = 13) uniform int activePointLights;
layout(location = 14) uniform vec3 pointLightPositions[20];
layout(location = 34) uniform vec3 pointLightColors[20];

void main(void)
{
    float a = materialShine;

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

    for (int i = 0; i < activePointLights; ++i)
    {
        // Point light
        vec3 pointLightPositionView = vec3(worldView * vec4(pointLightPositions[i], 1.0));
        float d = distance(position, pointLightPositionView);
        float f = 1.0 / (1 + 0.001 * d * d);
        ambient += materialAmbient * pointLightColors[i] * f;
        s = normalize(vec3(worldView * vec4(pointLightPositions[i], 1.0)) - position);
        cos_theta = max(dot(s, n), 0);
        diffuse += materialDiffuse * pointLightColors[i] * cos_theta * f;
    }

    vec3 result = ambient + diffuse;
    result.r = min(result.r, 1);
    result.g = min(result.g, 1);
    result.b = min(result.b, 1);

    outColor = vec4(result, 1.0);
}