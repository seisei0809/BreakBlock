#include "ShaderHeader.hlsli"

float4 PS(Output input) : SV_TARGET
{
    // uv値のピクセルがサンプラーで加工される
    float4 texColor = tex.Sample(smp, input.uv);
    // 色を適用して返す
    return texColor * color;
}
