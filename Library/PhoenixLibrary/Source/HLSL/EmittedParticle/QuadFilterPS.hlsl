
Texture2D textureMap : register(t0);
SamplerState samplerState : register(s0);

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

cbuffer FilterCB : register(b0)
{
    float bright; //���x
    float contrast; //�Z�W
    float saturate; //�ʓx
    float option;
    float4 screenColor; //�F��
}

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = textureMap.Sample(samplerState, pin.texcoord) * pin.color;
    
    // ���x����
    color.rgb += bright;
    
    // �R���g���X�g����
    color.rgb = ((color.rgb - 0.5f) * contrast) + 0.5f;
    
    // �ʓx����
    float avr = (color.r + color.g + color.b) / 3;
    color.rgb = (color.rgb - avr) * saturate + avr;
    
    // �J���[�o�����X����
    color.rgb *= screenColor.rgb;
    
    return color;
}