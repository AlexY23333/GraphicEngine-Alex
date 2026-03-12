// Description: standard effect applies to lighting

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix wvpInv;
    matrix world;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialEmissive;
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;

    float materialShininess;
}

cbuffer SettingsBuffer : register(b3)
{
    bool useDiffuseMap;
    bool useNormalMap;
    bool useSpecMap;
    bool useBumpMap;
    bool useShadowMap;
    bool useSkinning;
    float bumpWeight;
    float depthBias;
}

cbuffer BoneTransformBuffer : register(b4)
{
    matrix boneTransforms[256];
}

Texture2D diffuseMap : register(t0);

Texture2D normalMap : register(t1);

Texture2D specularMap : register(t2);

Texture2D bumpMap : register(t3);

Texture2D shadowMap : register(t4);

SamplerState textureSampler : register(s0);

static matrix Identity =
{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
    if (length(weights) <= 0.0f)
    {
        return Identity;
    }
    
    matrix transform = boneTransforms[indices[0]] * weights[0];
    transform += boneTransforms[indices[1]] * weights[1];
    transform += boneTransforms[indices[2]] * weights[2];
    transform += boneTransforms[indices[3]] * weights[3];
    
    return transform;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
    int4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float2 texCoord : TEXCOORD;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
};

VS_OUTPUT VS(VS_INPUT input)
{
    matrix toWorld = world;
    matrix toNDC = wvp;
    matrix toLNDC = wvpInv;
    
    
   
    if (useSkinning)
    {
        matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
        toWorld = mul(boneTransform, toWorld);
        toNDC = mul(boneTransform, toNDC);
    }
    float3 localPosition = input.position;
    
    if (useBumpMap)
    {
        float4 bumpMapColor = bumpMap.SampleLevel(textureSampler, input.texCoord, 0.0f);
        float bumpColor = (2.0f * bumpMapColor.r) - 1.0f;
        localPosition += (input.normal * bumpColor * bumpWeight);
    }
    
    
    VS_OUTPUT output;
    output.position = mul(float4(localPosition, 1.0f), toNDC);
    output.worldNormal = mul(input.normal, (float3x3) toWorld);
    output.worldTangent = mul(input.tangent, (float3x3) toWorld);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - (mul(float4(localPosition, 1.0f), toWorld).xyz));
    if (useShadowMap)
    {
        output.lightNDCPosition = mul(float4(localPosition, 1.0f), toLNDC);

    }
    
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    if (useNormalMap)
    {
        float3 t = normalize(input.worldTangent);
        float3 b = normalize(cross(n, t));
        float3x3 tbnw = float3x3(t, b, n);
        float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);
        float3 unpackedNormalMap = normalize(float3((normalMapColor.xy * 2.0f) - 1.0f, normalMapColor.z));
        n = normalize(mul(unpackedNormalMap, tbnw));
    }

    
    // emissive
    float4 emissive = materialEmissive;
    
    // ambient
    float4 ambient = lightAmbient * materialAmbient;
    
    // diffuse
    float4 d = saturate(dot(n, light));
    float4 diffuse = d * lightDiffuse * materialDiffuse;
    
    // specular
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialShininess);
    float4 specular = s * lightSpecular * materialSpecular;
    
    // texture color
    float4 diffuseColor = (useDiffuseMap) ? diffuseMap.Sample(textureSampler, input.texCoord) : 1.0f;
    float4 specMapColor = (useSpecMap) ? specularMap.Sample(textureSampler, input.texCoord).r : 1.0f;
    
    // final color
    float4 finalColor = (emissive + ambient + diffuse) * diffuseColor + (specular * specMapColor);
    
    // shadow
    if (useShadowMap)
    {
        float actualDepth = 1.0f - (input.lightNDCPosition.z / input.lightNDCPosition.w);
        float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
        float u = (shadowUV.x + 1.0f) * 0.5f;
        float v = 1.0f - (shadowUV.y + 1.0f) * 0.5f;
        if (saturate(u) == u && saturate(v) == v)    // if it is in view area
        {
            float4 savedColor = shadowMap.Sample(textureSampler, float2(u, v));
            float savedDepth = savedColor.r;
            if (savedDepth > actualDepth + depthBias)
            {
                finalColor = (emissive + ambient) * diffuseColor;
            }
        }
    }
    return finalColor;
}