// Description: base fx file for standard object rendering
// the local space vertices into NDC space

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
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
    float bumpWeight;
}


Texture2D diffuseMap : register(t0);

Texture2D normalMap : register(t1);

Texture2D specularMap : register(t2);

Texture2D bumpMap : register(t3);


SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float2 texCoord : TEXCOORD;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT input)
{
    float3 localPosition = input.position;
    
    
    VS_OUTPUT output;
    output.position = mul(float4(localPosition, 1.0f), wvp);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - (mul(float4(localPosition, 1.0f), world).xyz));
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    // emissive
    float edgeThickness = 0.85f;
    float edgeThreshold = 0.01f;
    float e = 1.0f - saturate(dot(view, n));
    e = smoothstep(edgeThickness - 0.01f, edgeThickness + 0.01f, e);
    float4 emissive =e* materialEmissive;
    
    // ambient
    float4 ambient = lightAmbient * materialAmbient;
    
    // diffuse
    float4 d = saturate(dot(n, light));
    d = smoothstep(0.005f, 0.1f, d);
    float4 diffuse = d * lightDiffuse * materialDiffuse;
    
    // specular
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialShininess);
    s = smoothstep(0.005f, 0.1f, s);
    float4 specular = s * lightSpecular * materialSpecular;
    
    // texture color
    float4 diffuseColor = 1.0f;
    float4 specMapColor = 1.0f;
    
    // final color
    float4 finalColor = (emissive + ambient + diffuse) * diffuseColor + (specular * specMapColor);
    
    return finalColor;
}