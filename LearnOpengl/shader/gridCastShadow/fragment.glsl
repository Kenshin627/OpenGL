#version 330 core

uniform vec3 lightDirection;
uniform sampler2D shadowMap;

in vec3 vNormal;
in vec4 fragPosLightSpace;

float calcShadow()
{
    vec3 fragCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    fragCoords = fragCoords * 0.5 + 0.5;
    if(fragCoords.z > 1.0)
    {
        return 0.0;
    }

    //PCF
    vec2 texel = 1.0 / textureSize(shadowMap, 0);
    float shadow = 0.0;
    vec3 n = normalize(vNormal);
    vec3 lightDirectionReverse = -normalize(lightDirection);
    float shadowBias = max(0.002 * (1.0 - dot(lightDirectionReverse, n)), 0.0002);
    for(int x = -1; x <= 1; x++ )
    {
        for(int y = -1; y <= 1; y++)
        {
            float closetDepth = texture(shadowMap, fragCoords.xy + vec2(x,y) * texel).r;
            shadow += ((fragCoords.z  - shadowBias) > closetDepth? 1.0 : 0.0); 
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main(void) {    
    float shadow = calcShadow();
    gl_FragColor = vec4(vec3(0.7) * (1.0 - shadow), 1.0);
}