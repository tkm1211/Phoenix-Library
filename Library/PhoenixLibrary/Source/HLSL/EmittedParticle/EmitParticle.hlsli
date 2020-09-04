
struct ParticleHeader
{
    uint tag; // alive + emitterID + index
    float depth;
};


cbuffer ParticleCB : register(b0)
{
    uint TotalSpawnCount;
};


static const uint IA_PARTICLE_COUNTER = 0;
static const uint IA_PREV_PARTICLE_COUNTER = IA_PARTICLE_COUNTER + 4;

static const uint PARTICLE_PER_THREAD = 256; // Dispatch(ceil(TotalParticleMax / (float)PARTICLE_PER_THREAD), 1 ,1)

static const uint INVALID_TAG = 0xffffffff;

static const uint TotalEmitterMax = 256;
static const uint TotalParticleMax = 1024;