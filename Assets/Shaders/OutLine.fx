// Outline.fx
cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewPosition;
}

cbuffer SettingsBuffer : register(b1)
{
    float outlineWidth;
    float3 outlineColor;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldPosition : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = normalize(mul(input.normal, (float3x3) world));
    output.worldPosition = mul(float4(input.position, 1.0f), world).xyz;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 viewDir = normalize(viewPosition - input.worldPosition);
    float edgeFactor = dot(n, viewDir);
    
    if (edgeFactor < outlineWidth)
    {
        return float4(outlineColor, 1.0);
    }
    
    discard;
    return float4(0, 0, 0, 0);
}
