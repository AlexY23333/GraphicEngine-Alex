// Outline.fx (Soft Gradient Outline)

// ================================
// Constant Buffers
// ================================
cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewPosition;
    float _pad0; // padding for 16-byte alignment
};

cbuffer SettingsBuffer : register(b1)
{
    float outlineWidth; // 轮廓区域阈值（0~1，越大轮廓越宽）
    float3 outlineColor; // 轮廓颜色
};

// ================================
// Vertex I/O
// ================================
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
};

// ================================
// Vertex Shader
// ================================
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    // Clip-space position
    output.position = mul(float4(input.position, 1.0f), wvp);

    // World-space normal (note: for non-uniform scale, inverse-transpose is more correct)
    output.worldNormal = normalize(mul(input.normal, (float3x3) world));

    // World-space position
    output.worldPosition = mul(float4(input.position, 1.0f), world).xyz;

    return output;
}

// ================================
// Pixel Shader (Soft Gradient)
// ================================
float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 v = normalize(viewPosition - input.worldPosition);

    // edgeFactor: 0 = near silhouette, 1 = facing camera
    float edgeFactor = saturate(dot(n, v));

    // Only draw within outline region; outside discard
    if (edgeFactor >= outlineWidth)
        discard;

    // -------- Soft fade --------
    // 我们希望：
    // - 轮廓最外缘 (edgeFactor 接近 0) -> 强度接近 1
    // - 轮廓内边界 (edgeFactor 接近 outlineWidth) -> 强度接近 0，并且过渡柔和
    //
    // softRange 控制“软边宽度”，值越大越柔和（建议 0.10~0.30）
    const float softRange = 0.20;

    // 把 edgeFactor 映射到 0~1：0 在内边界，1 在外边缘
    // raw = 1 - edgeFactor/outlineWidth
    float raw = 1.0f - (edgeFactor / max(outlineWidth, 1e-5));

    // 用 smoothstep 做柔和曲线：
    // - 小于 0 -> 0
    // - 大于 1 -> 1
    // - 中间平滑过渡
    //
    // 这里用 (0, softRange) 做一个“更宽的柔和起步”
    float intensity = smoothstep(0.0f, softRange, raw);

    // 你也可以加一个轻微的“尾部拉长”，让外缘更柔和：
    // intensity = smoothstep(0.0f, softRange, raw) * smoothstep(0.0f, 1.0f, raw);

    return float4(outlineColor * intensity, 1.0f);
}
