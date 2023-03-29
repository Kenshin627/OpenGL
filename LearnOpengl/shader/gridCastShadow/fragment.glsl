#version 330 core

#define SQRT2 1.41421356
#define PI 3.14159

uniform vec3 mainColor;
uniform vec3 lineColor;
uniform vec4 gridControl;
uniform vec3 gridOffset;

uniform vec3 lightDirection;
uniform sampler2D shadowMap;

in vec3 vPosition;
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

float getVisibility(float position) {
    // Major grid line every Frequency defined in material.
    float majorGridFrequency = gridControl.y;
    if (floor(position + 0.5) == floor(position / majorGridFrequency + 0.5) * majorGridFrequency)
    {
        return 1.0;
    }  
    return gridControl.z;
}

float getAnisotropicAttenuation(float differentialLength) {
    const float maxNumberOfLines = 10.0;
    return clamp(1.0 / (differentialLength + 1.0) - 1.0 / maxNumberOfLines, 0.0, 1.0);
}

float isPointOnLine(float position, float differentialLength) {
    float fractionPartOfPosition = position - floor(position + 0.5); // fract part around unit [-0.5; 0.5]
    fractionPartOfPosition /= differentialLength; // adapt to the screen space size it takes
    fractionPartOfPosition = clamp(fractionPartOfPosition, -1., 1.);
    
    float result = 0.5 + 0.5 * cos(fractionPartOfPosition * PI); // Convert to 0-1 for antialiasing.
    return result;    
}

float contributionOnAxis(float position) {
    float differentialLength = length(vec2(dFdx(position), dFdy(position)));
    differentialLength *= SQRT2;  // Multiply by SQRT2 for diagonal length
    
    // Is the point on the line.
    float result = isPointOnLine(position, differentialLength);

    // Add dynamic visibility.
    float visibility = getVisibility(position);
    result *= visibility;
    
    // Anisotropic filtering.
    float anisotropicAttenuation = getAnisotropicAttenuation(differentialLength);
    result *= anisotropicAttenuation;
    
    return result;
}

float normalImpactOnAxis(float x) {
    float normalImpact = clamp(1.0 - 3.0 * abs(x * x * x), 0.0, 1.0);
    return normalImpact;
}

void main(void) {    
    float shadow = calcShadow();
    float gridRatio = gridControl.x;
    vec3 gridPos = (vPosition + gridOffset.xyz) / gridRatio;

    float x = contributionOnAxis(gridPos.x);
    float y = contributionOnAxis(gridPos.y);
    float z = contributionOnAxis(gridPos.z);
    vec3 normal = normalize(vNormal);
    x *= normalImpactOnAxis(normal.x);
    y *= normalImpactOnAxis(normal.y);
    z *= normalImpactOnAxis(normal.z);
    float grid = clamp(x + y + z, 0., 1.);
    vec3 color = mix(mainColor, lineColor, grid);
    gl_FragColor = vec4(color.rgb * (1.0 - shadow), 1.0);
}