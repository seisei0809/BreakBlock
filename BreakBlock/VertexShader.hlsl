#include "ShaderHeader.hlsli"

Output VS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    Output output;                  // RS�֏o�͗p
    output.svpos = mul(pos, mat);   // �ϊ��s��Ń��[�J�����W->NDC���W�ɕϊ�
    output.uv = uv;                 // uv�͂��̂܂�
    return output;                  // �o��
}