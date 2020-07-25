//#define USE_POSITION
#define USE_NORMAL
#define USE_UVW

#define CB_SCENE	b0
#define CB_MESH		b1

#include "CommonCB.hlsli"
#include "CommonLayout.hlsli"


VS_OUTPUT main(VS_INPUT input)
{
	// Set up output
    VS_OUTPUT output = (VS_OUTPUT) 0;

	// Make a view matrix with NO translation
    matrix viewNoMovement = cb_view;
    viewNoMovement._41 = 0;
    viewNoMovement._42 = 0;
    viewNoMovement._43 = 0;

	// Calculate output position
    matrix viewProj = mul(viewNoMovement, cb_projection);
    output.sv_position = mul(float4(input.position.xyz, 1.0f), viewProj);

	// Ensure our polygons are at max depth
    output.sv_position.z = output.sv_position.w;

	// Use the cube's vertex position as a direction in space
	// from the origin (center of the cube)
    output.uvw = input.position;

    return output;
}