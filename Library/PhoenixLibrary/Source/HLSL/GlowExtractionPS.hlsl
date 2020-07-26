#include "bloom.hlsli"


Texture2D hdrColorMap : register(t0);

SamplerState pointSamplerState : register(s0);
SamplerState linearSamplerState : register(s1);
SamplerState anisotropicSamplerState : register(s2);


float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    float4 colorMapColor = hdrColorMap.Sample(pointSamplerState, texcoord);
    
    float luminance = dot(colorMapColor.rgb, float3(0.2126, 0.7152, 0.0722));
    
    return luminance > glowExtractionThreshold ? float4(colorMapColor.rgb, 1.0) : float4(0.0, 0.0, 0.0, 1.0);
}