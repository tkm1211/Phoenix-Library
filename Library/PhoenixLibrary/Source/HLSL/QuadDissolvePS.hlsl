
Texture2D textureMap : register(t0);
Texture2D maskTexture : register(t1);
Texture2D maskTexture02 : register(t2);
Texture2D emissiveTexture : register(t3);

SamplerState samplerState : register(s0);

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

cbuffer DissolveCB : register(b0)
{
    float dissolveThreshold; //����臒l
    float dissolveEmissiveWidth; //����臒l(�f�B�]���u�E�G�~�b�V�u)
    float2 dummy;
}

float4 main(VS_OUT pin) : SV_TARGET
{
    //�e�N�X�`���摜
    float4 color = textureMap.Sample(samplerState, pin.texcoord) * pin.color;
    
    //�}�X�N�̖���
    float mask = maskTexture.Sample(samplerState, pin.texcoord).r;
    float mask02 = maskTexture02.Sample(samplerState, pin.texcoord).r;
    mask *= mask02;
    
    //臒l���}�X�N�̒l�𒴂��Ă����犮�S���߁A�����Ă��Ȃ�������s���߂�2�l����
    //�������l�����̏ꍇ��0.0���A����ȏ�̏ꍇ��1.0�̓�l�����s���֐�
    float thresold = 1.0f - dissolveThreshold;
    float alpha = step(mask, thresold);
    
    // �����ӏ��v�Z
    float3 emissive = (float3) 0;
    float3 emissive_color = emissiveTexture.Sample(samplerState, pin.texcoord).rgb;
    float emissive_value = (mask + dissolveEmissiveWidth > thresold) && (thresold > mask);
    emissive = emissive_color * emissive_value;
   
    color.rgb += emissive;
    color.a *= alpha;

    return color;
}