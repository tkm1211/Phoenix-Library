#define USE_UVW

#include "CommonLayout.hlsli"

typedef VS_OUTPUT PS_INPUT;

#include "CommonCB.hlsli"

// External texture-related data
TextureCube Sky : register(t0);
SamplerState basicSampler : register(s0);


// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
    return Sky.Sample(basicSampler, input.uvw);
}