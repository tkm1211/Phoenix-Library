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

cbuffer DissolveCB : register(b1)
{
    float dissolveThreshold; //����臒l
    float dissolveEmissiveWidth; //����臒l(�f�B�]���u�E�G�~�b�V�u)
    float2 dummy;
}

//--------------------------------------------------------------------------------------
// ���\�[�X
//--------------------------------------------------------------------------------------
Texture2D texture0 : register(t0);
Texture2D maskTexture : register(t10);
SamplerState sampler0 : register(s0);

//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 ShadowPos = float4(input.position, 1.0);
    
    //�}�X�N�̖���
    float mask = maskTexture.Sample(sampler0, input.texcoord0).r;
    
    //臒l���}�X�N�̒l�𒴂��Ă����犮�S���߁A�����Ă��Ȃ�������s���߂�2�l����
    //�������l�����̏ꍇ��0.0���A����ȏ�̏ꍇ��1.0�̓�l�����s���֐�
    float thresold = 1.0f - dissolveThreshold;
    float alpha = step(mask, thresold);
    
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
    
    float4 color = texture0.Sample(sampler0, input.texcoord0) * float4(shadow, 1.0f) * cbColor;
    color.a *= alpha;
    
    return color;
}