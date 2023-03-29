#version 330 core

uniform vec3 lightDirection;
uniform sampler2D shadowMap;

in vec3 vNormal;
in vec4 fragPosLightSpace;

float calcShadow()
{
    vec3 fragCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    fragCoords = fragCoords * 0.5 + 0.5;
    float closetDepth = texture(shadowMap, fragCoords.xy).r;
    vec3 n = normalize(vNormal);
    vec3 lightDirectionReverse = -normalize(lightDirection);
    float shadowBias = min(0.005, (1.0 - max(dot(lightDirectionReverse, n), 0.0)) * 0.05);
    return (fragCoords.z - shadowBias) > closetDepth? 1.0 : 0.0;
}

void main(void) {    
    float shadow = calcShadow();
    gl_FragColor = vec4(vec3(0.7) * (1.0 - shadow), 1.0);
}