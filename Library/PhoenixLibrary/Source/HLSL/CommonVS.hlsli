#ifndef COMMON_VS_HLSLI
#define COMMON_VS_HLSLI

#define CB_SCENE	b0
#define CB_MESH		b1
#define CB_BONE		b2
//#define CB_SHADOW	b3

#include "CommonCB.hlsli"
#include "CommonLayout.hlsli"
#include "Skinning.hlsli"

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT VSCommon(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

#if defined(USE_BONE)
	input.position.xyz = SkinningPosition(input.position, input.blend_index0, input.blend_index1, input.blend_weight0, input.blend_weight1);
#if defined(USE_NORMAL)
	input.normal = SkinningVector(input.normal, input.blend_index0, input.blend_index1, input.blend_weight0, input.blend_weight1);
#endif
#if defined(USE_TANGENT)
	input.tangent = SkinningVector(input.tangent, input.blend_index0, input.blend_index1, input.blend_weight0, input.blend_weight1);
#endif
#endif

    float3 position = mul(input.position, cb_world).xyz;
    output.sv_position = mul(float4(position, 1.0f), cb_view_projection);

#if defined(USE_POSITION)
	output.position = position;
#endif
#if defined(USE_NORMAL)
	output.normal = mul(float4(input.normal, 0.0f), cb_world_inverse).xyz;
#endif
#if defined(USE_TANGENT)
	output.tangent = mul(float4(input.tangent, 0.0f), cb_world_inverse).xyz;
#endif

#if defined(USE_COLOR)
	output.color = input.color;
#endif

#if defined(USE_TEXCOORD0)
	output.texcoord0 = mul(float4(input.texcoord0.x, 1.0f - input.texcoord0.y, 0.0f, 1.0f), cb_texture).xy;
#endif
#if defined(USE_TEXCOORD1)
	output.texcoord1 = float2(input.texcoord1.x, 1.0f - input.texcoord1.y);
#endif
#if defined(USE_TEXCOORD2)
	output.texcoord2 = float2(input.texcoord2.x, 1.0f - input.texcoord2.y);
#endif
#if defined(USE_TEXCOORD3)
	output.texcoord3 = float2(input.texcoord3.x, 1.0f - input.texcoord3.y);
#endif

#if defined(USE_SHADOW)
	//output.shadow = mul(input.position, mul(cb_world, cb_shadow));
#endif
    return output;
}

#endif