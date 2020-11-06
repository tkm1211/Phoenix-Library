
#define CB_SCENE b2

#include "EmittedParticle.hlsli"
#include "../CommonCB.hlsli"

static const float3 BILLBOARD[] =
{
    float3(-1, -1, 0), // 0
	float3(1, -1, 0), // 1
	float3(-1, 1, 0), // 2
	float3(-1, 1, 0), // 3
	float3(1, -1, 0), // 4
	float3(1, 1, 0), // 5
};

StructuredBuffer<Particle> particleBuffer: register(t0);
StructuredBuffer<uint> aliveList : register(t1);


VertextoPixel main(uint fakeIndex : SV_VERTEXID)
{
    VertextoPixel Out;

	// bypass the geometry shader, and instead expand the particle here in the VS:
    uint vertexID = fakeIndex % 6;
    uint instanceID = fakeIndex / 6;

	// load particle data:
    Particle particle = particleBuffer[aliveList[instanceID]];

	// calculate render properties from life:
    float lifeLerp = 1 - particle.life / particle.maxLife;
    float size = lerp(particle.sizeBeginEnd.x, particle.sizeBeginEnd.y, lifeLerp);
    float opacity = saturate(lerp(1, 0, lifeLerp) * xEmitterOpacity);
    float rotation = particle.rotationalVelocity;

	// expand the point into a billboard in view space:
    float3 quadPos = BILLBOARD[vertexID];
    float2 uv = quadPos.xy * float2(0.5f, -0.5f) + 0.5f;
    uv.x = particle.colorMirror & 0x10000000 ? 1.0f - uv.x : uv.x;
    uv.y = particle.colorMirror & 0x20000000 ? 1.0f - uv.y : uv.y;
	
	// rotate the billboard:
    float2x2 rot = float2x2(
		cos(rotation), -sin(rotation),
		sin(rotation), cos(rotation)
		);
    quadPos.xy = mul(quadPos.xy, rot);

	// scale the billboard:
    quadPos *= size;

	// scale the billboard along view space motion vector:
    float3 velocity = mul((float3x3)cb_view, particle.motionVelocity.xyz);
    quadPos += dot(quadPos, velocity) * velocity * xParticleMotionBlurAmount;

	// copy to output:
    Out.pos = float4(particle.position, 1);
    Out.pos = mul(cb_view, Out.pos);
    Out.pos.xyz += quadPos.xyz;
    Out.P = mul(cb_viewInv, float4(Out.pos.xyz, 1)).xyz;
    Out.pos = mul(cb_projection, Out.pos);

    Out.tex = uv;
    Out.size = size;
    Out.color = (particle.colorMirror & 0x00FFFFFF) | (uint(opacity * 255.0f) << 24);
    Out.pos2D = Out.pos;
    
    Out.life = particle.life;

    return Out;
}