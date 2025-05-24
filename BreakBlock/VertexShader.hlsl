#include "ShaderHeader.hlsli"

Output VS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    Output output;                  // RSへ出力用
    output.svpos = mul(pos, mat);   // 変換行列でローカル座標->NDC座標に変換
    output.uv = uv;                 // uvはそのまま
    return output;                  // 出力
}