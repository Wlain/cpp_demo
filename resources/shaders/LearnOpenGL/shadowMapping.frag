#version 330 core
out vec4 fragColor;

in VsOut {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragPosLightSpace;
} fsIn;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float calcShadow(vec4 fragPoseLightSpace)
{
    // 透视除法
    vec3 projCoords = fragPoseLightSpace.xyz / fragPoseLightSpace.w;
    // [-1, 1] -> [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度，使用[0,1]范围下的fragPosLight当坐标
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = fsIn.normal;
    vec3 lightDir = normalize(lightPos - fsIn.fragPos);
    // 纹理偏移
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // PCF
    float shadow = 0.0;
    /// textureSize返回一个给定采样器纹理的0级mipmap的vec2类型的宽和高
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // 解决clamp_to_border不起作用问题
    if (projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
    // 1：代表在阴影内，0:代表在阴影外
    return shadow;
}

void main() {
    vec3 color = texture(diffuseTexture, fsIn.texCoords).rgb;
    vec3 normal = fsIn.normal;
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fsIn.fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fsIn.fragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // calculate shadow
    float shadow = calcShadow(fsIn.fragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    fragColor = vec4(lighting, 1.0);
}
