struct Output
{
    float4 svpos : SV_Position; // �V�X�e���p���_���W
    float2 uv : TEXCOORD;       // uv�l
};

Texture2D<float4> tex : register(t0);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
// �萔�o�b�t�@
cbuffer cbuff0 : register(b0)
{
    matrix mat;     // �ϊ��s��
    float4 color;   // RGBA
}