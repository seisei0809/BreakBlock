#include "ShaderHeader.hlsli"

float4 PS(Output input) : SV_TARGET
{
    // uv�l�̃s�N�Z�����T���v���[�ŉ��H�����
    float4 texColor = tex.Sample(smp, input.uv);
    // �F��K�p���ĕԂ�
    return texColor * color;
}
