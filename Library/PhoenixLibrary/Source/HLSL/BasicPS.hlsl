#define USE_POSITION
#define USE_TEXCOORD0
#include "CommonLayout.hlsli"
#include "ShadowMap.hlsli"

//--------------------------------------------------------------------------------------
// ���̓��C�A�E�g
//--------------------------------------------------------------------------------------
typedef VS_OUTPUT PS_INPUT;

//--------------------------------------------------------------------------------------
// �萔�o�b�t�@
//--------------------------------------------------------------------------------------
cbuffer CbMaterial : register(b0)
{
    float4 cbColor;
};

//--------------------------------------------------------------------------------------
// ���\�[�X
//--------------------------------------------------------------------------------------
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 ShadowPos = float4(input.position, 1.0);
    
	//�V���h�[��ԍ��W
    ShadowPos = mul(ShadowPos, lightViewProjection);
    ShadowPos.xyz /= ShadowPos.w;
    ShadowPos.y = -ShadowPos.y;
    
	//�V���h�[�}�b�v���
    ShadowPos.xy = ShadowPos.xy * 0.5 + 0.5;
    
    float3 shadow = (float3) 1.0;
    if (0.0f < ShadowPos.x && ShadowPos.x < 1.0f && 0.0f < ShadowPos.y && ShadowPos.y < 1.0f)
    {
        //�V���[�h�[�}�b�v�̐[�x
        float depth = shadowMap.Sample(sampler0, ShadowPos.xy).r;

        if (ShadowPos.z > depth + 0.0005)
        {
            shadow = /*1.0 - (ShadowPos.z - depth) / ShadowPos.z*/0.5f;
        }
    }
    
    return texture0.Sample(sampler0, input.texcoord0) * float4(shadow, 1.0f) * cbColor;
}