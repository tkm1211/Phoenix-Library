
#include "EmittedParticle.hlsli"


RWStructuredBuffer<Particle> particleBuffer : register(u0);
RWStructuredBuffer<uint> aliveBuffer_CURRENT : register(u1);
RWStructuredBuffer<uint> aliveBuffer_NEW : register(u2);
RWStructuredBuffer<uint> deadBuffer : register(u3);
RWByteAddressBuffer counterBuffer : register(u4);
RWStructuredBuffer<float> distanceBuffer : register(u6);


[numthreads(THREADCOUNT_SIMULATION, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID, uint Gid : SV_GroupIndex)
{
    // simulation can be either fixed or variable timestep:
    const float dt = xEmitterFixedTimestep; // TODO : dt
    
	//uint aliveCount = counterBuffer[0].aliveCount;
    uint aliveCount = counterBuffer.Load(PARTICLECOUNTER_OFFSET_ALIVECOUNT);
    if (DTid.x < aliveCount)
    {
        uint particleIndex = aliveBuffer_CURRENT[DTid.x];
        Particle particle = particleBuffer[particleIndex];
        
        if (particle.life > 0)
        {
		    // integrate:
            particle.velocity += 10.0f * dt;
            particle.position += particle.velocity * dt;

		    // reset force for next frame:
            particle.force = 0;
            particle.life -= dt;

		    // write back simulated particle:
            particleBuffer[particleIndex] = particle;

		    // add to new alive list:
            uint newAliveIndex;
            counterBuffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION, 1, newAliveIndex);
            aliveBuffer_NEW[newAliveIndex] = particleIndex;
        }
        else
        {
            // kill:
            uint deadIndex;
            counterBuffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_DEADCOUNT, 1, deadIndex);
            deadBuffer[deadIndex] = particleIndex;
        }
    }
}
