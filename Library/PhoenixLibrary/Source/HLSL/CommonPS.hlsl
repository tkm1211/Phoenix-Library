#ifndef COMMON_PS_HLSLI
#define COMMON_PS_HLSLI


//--------------------------------------------------------------------------------------
// �\����
//--------------------------------------------------------------------------------------
struct DirLight
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float3 dir;
};

struct PointLight
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float3 att;
    float4 pos; // xyz, range
};

struct BoxLight
{
    DirLight dir;
    float3 min;
    float3 max;
    float3 att;
};

struct LightEnvironment
{
    DirLight dirLight[2];
    BoxLight boxLight[2];
    PointLight pointLight[4];
    uint numDirLights;
    uint numBoxLights;
    uint numPointLights;
    uint pad1;
};

struct ExpFog
{
    int enabled;
    float density;
    float range;
    float fpad;
    float4 color;
};

//--------------------------------------------------------------------------------------
// ���C�e�B���O
//--------------------------------------------------------------------------------------
void ComputePointLight(
	float3 pos,
	float3 normal,
	float3 toEye,
	float specPower,
	PointLight pl,
	out float3 ambient,
	out float3 diffuse,
	out float3 specular)
{
    ambient = float3(0.0f, 0.0f, 0.0f);
    diffuse = float3(0.0f, 0.0f, 0.0f);
    specular = float3(0.0f, 0.0f, 0.0f);

    float range = pl.pos.w;

	// from surface to light
    float3 toLight = pl.pos.xyz - pos;

	// The distance from surface to light.
    float dist = length(toLight);

    if (dist > range)
        return;

	// normalize tolight vector
    toLight = toLight / dist;

	// compute diffuse factor
    float df = dot(toLight, normal);
	[branch]
    if (df > 0.0f)
    {
        float3 rv = reflect(-toLight, normal);
        float sf = pow(max(dot(rv, toEye), 0.0f), specPower);

		// diffuse and specular component
        diffuse = df * pl.diffuse;
        specular = sf * pl.specular;

    }

	// Attenuate
    float t = 1.0 - dist / range;
    float att = dot(pl.att, float3(1.0f, t, t * t));
    diffuse *= att;
    specular *= att;
    ambient = pl.ambient * att;
}

void ComputeDirLight(
	float3 pos,
	float3 normal,
	float3 toEye,
	float specPower,
	DirLight dl,
	out float3 ambient,
	out float3 diffuse,
	out float3 specular)
{

    ambient = dl.ambient;
    diffuse = float3(0.0f, 0.0f, 0.0f);
    specular = float3(0.0f, 0.0f, 0.0f);


    float3 tolight = -dl.dir;

	// diffuse factor
    float df = dot(normal, tolight);

	[branch]
    if (df > 0)
    {
        diffuse = df * dl.diffuse;

		// reflection vector
        float3 rv = reflect(dl.dir, normal);
        float sf = pow(max(dot(rv, toEye), 0.0f), specPower);
        specular = sf * dl.specular;

    }
}

float MinElm(float3 t)
{
    float x = t.x;
    if (t.y < x)
    {
        x = t.y;
    }
    if (t.z < x)
    {
        x = t.z;
    }
    return x;
}

void ComputeBoxLight(
	float3 pos,
	float3 normal,
	float3 toEye,
	float specPower,
	BoxLight dl,
	out float3 ambient,
	out float3 diffuse,
	out float3 specular)
{
    ambient = float3(0.0f, 0.0f, 0.0f);
    diffuse = float3(0.0f, 0.0f, 0.0f);
    specular = float3(0.0f, 0.0f, 0.0f);

    float t = 1.0;
    if (pos.x < dl.min.x || pos.y < dl.min.y || pos.z < dl.min.z)
    {
        return;
    }
    if (pos.x > dl.max.x || pos.y > dl.max.y || pos.z > dl.max.z)
    {
        return;
    }

    ComputeDirLight(pos, normal, toEye, specPower, dl.dir, ambient, diffuse, specular);

	// calculate distance to edge of AABB
    float3 range = (dl.max - dl.min) * float3(.5, .5, .5);
    float3 center = dl.min + range;
    float3 dist = abs(center - pos);

	// normalize 1.0 to 0.0
    float3 ts = float3(1.0, 1.0, 1.0) - dist / range;
    t = MinElm(ts);


    float att = dot(dl.att, float3(1.0f, t, t * t));
    diffuse *= att;
    specular *= att;

}

float ComputeShadowFactor(
	Texture2D depthmap,
	SamplerComparisonState depthmapSampler,
	float3 texShadow,
	float texelSize)
{
    float fPercentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-texelSize, -texelSize), float2(0.0f, -texelSize), float2(texelSize, -texelSize),
		float2(-texelSize, 0.0f), float2(0.0f, 0.0f), float2(texelSize, 0.0f),
		float2(-texelSize, texelSize), float2(0.0f, texelSize), float2(texelSize, texelSize)
    };

	[unroll]
    for (int i = 0; i < 9; ++i)
    {
        fPercentLit += depthmap.SampleCmpLevelZero(depthmapSampler, texShadow.xy + offsets[i], texShadow.z).r;
    }

    return (fPercentLit / 9.0f);
}

void ComputeLighting(
	float3 pos,
	float3 normal,
	float3 eyePos,
	float specPower,
	float shadowFactor,
	LightEnvironment env,
	out float3 ambient,
	out float3 diffuse,
	out float3 specular)
{

	// init out params
    ambient = float3(0.0f, 0.0f, 0.0f);
    diffuse = float3(0.0f, 0.0f, 0.0f);
    specular = float3(0.0f, 0.0f, 0.0f);

    float4 shadowFactors = float4(shadowFactor, 1, 1, 1);
    float3 toEye = normalize(eyePos - pos);
    uint numDirLights = min(4, env.numDirLights);

    float3 amb, diff, spec;

    uint i;
    for (i = 0; i < numDirLights; i++)
    {
        ComputeDirLight(pos, normal, toEye, specPower, env.dirLight[i], amb, diff, spec);
        ambient += amb;
        diffuse += diff * shadowFactors[i];
        specular += spec * shadowFactors[i];
    }

    for (i = 0; i < env.numBoxLights; i++)
    {
        ComputeBoxLight(pos, normal, toEye, specPower, env.boxLight[i], amb, diff, spec);
        ambient += amb;
        diffuse += diff;
        specular += spec;
    }

    for (i = 0; i < env.numPointLights; i++)
    {
        ComputePointLight(pos, normal, toEye, specPower, env.pointLight[i], amb, diff, spec);
        ambient += amb;
        diffuse += diff;
        specular += spec;

    }

}

//--------------------------------------------------------------------------------------
// �t�H�O
//--------------------------------------------------------------------------------------
float ComputeFogFactor(ExpFog fog, float distW)
{
    float rdist = distW / fog.range * 5;
    return exp(-fog.density * rdist);
}

#endif