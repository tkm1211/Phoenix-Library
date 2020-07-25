#define USE_POSITION
#define USE_NORMAL
#define USE_TANGENT
#define USE_TEXCOORD0
#define USE_PBR
#include "CommonLayout.hlsli"

// defines
#define PI 3.14159265359
#define PI2 6.28318530718
#define RECIPROCAL_PI 0.31830988618
#define RECIPROCAL_PI2 0.15915494
#define LOG2 1.442695
#define EPSILON 1e-6

#define LIGHT_MAX 4

struct GeometricContext
{
    float3 position;
    float3 normal;
    float3 viewDir;
};

struct Material
{
    float3 diffuseColor;
    float3 specularColor;
    float specularRoughness;
};

struct ReflectedLight
{
    float3 directDiffuse;
    float3 directSpecular;
    float3 indirectDiffuse;
    float3 indirectSpecular;
};

struct IncidentLight
{
    float3 direction;
    float3 color;
    bool visible;
};

struct DirectionalLight
{
    float4 direction;
    float4 color;
};

struct PointLight
{
    float3 position;
    float3 color;
    float distance;
    float decay;
};

struct SpotLight
{
    float3 position;
    float3 direction;
    float3 color;
    float distance;
    float decay;
    float coneCos;
    float penumbraCos;
};

//--------------------------------------------------------------------------------------
// 入力レイアウト
//--------------------------------------------------------------------------------------
typedef VS_OUTPUT PS_INPUT;

//--------------------------------------------------------------------------------------
// 定数バッファ
//--------------------------------------------------------------------------------------
cbuffer CbMaterial : register(b0)
{
    float4 albedo;
    float metallic;
    float roughness;
    float padding01;
    float padding02;
};

cbuffer CbScene : register(b1)
{
    DirectionalLight dirLight;
    float4 cb_eye;
};

//--------------------------------------------------------------------------------------
// リソース
//--------------------------------------------------------------------------------------
Texture2D albedoTex : register(t0);
Texture2D specularTex : register(t2);
Texture2D normalTex : register(t3);
Texture2D emissiveTex : register(t5);

SamplerState sampler0 : register(s0);

// Normalized Lambert
float3 DiffuseBRDF(float3 diffuseColor)
{
    return diffuseColor / PI;
}

float D_GGX(float a, float dotNH)
{
    float a2 = a * a;
    float dotNH2 = dotNH * dotNH;
    float d = dotNH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL)
{
    float k = a * a * 0.5 + EPSILON;
    float gl = dotNL / (dotNL * (1.0 - k) + k);
    float gv = dotNV / (dotNV * (1.0 - k) + k);
    return gl * gv;
}

float3 F_Schlick(float3 specularColor, float3 H, float3 V)
{
    return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

// Cook-Torrance
float3 SpecularBRDF(const in IncidentLight directLight, const in GeometricContext geometry, float3 specularColor, float roughnessFactor)
{
    float3 N = geometry.normal;
    float3 V = geometry.viewDir;
    float3 L = directLight.direction;

    float dotNL = saturate(dot(N, L));
    float dotNV = saturate(dot(N, V));
    float3 H = normalize(L + V);
    float dotNH = saturate(dot(N, H));
    float dotVH = saturate(dot(V, H));
    float dotLV = saturate(dot(L, V));
    float a = roughnessFactor * roughnessFactor;

    float D = D_GGX(a, dotNH);
    float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
    float3 F = F_Schlick(specularColor, V, H);
    return (F * (G * D)) / (4.0 * dotNL * dotNV + EPSILON);
}

void getDirectionalDirectLightIrradiance(const in DirectionalLight directionalLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    directLight.color = directionalLight.color.xyz;
    directLight.direction = directionalLight.direction.xyz;
    directLight.visible = true;
}

// RenderEquations(RE)
void RE_Direct(const in IncidentLight directLight, const in GeometricContext geometry, const in Material material, inout ReflectedLight reflectedLight)
{
    float dotNL = saturate(dot(geometry.normal, directLight.direction));
    float3 irradiance = dotNL * directLight.color;

    // punctual light
    irradiance *= PI;

    reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
    reflectedLight.directSpecular += irradiance * SpecularBRDF(directLight, geometry, material.specularColor, material.specularRoughness);
}

//--------------------------------------------------------------------------------------
// ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    // 接空間軸
    float3 vB = normalize(cross(input.tangent, input.normal));
    
    float3 vx = normalize(input.tangent);
    float3 vy = normalize(vB);
    float3 vz = normalize(input.normal);
    
    // テクスチャ色取得
    float3 N = normalTex.Sample(sampler0, input.texcoord0).xyz;
    N = N * 2.0f - 1.0f;
    
    // 法線ワールド変換
    float3x3 mat = { vx, vy, vz };
    N = normalize(mul(N, mat));
    
    GeometricContext geometry;
    //geometry.position = -lighPos.xyz;
    //geometry.normal = normalize(input.normal);
    geometry.normal = N;
    geometry.viewDir = normalize(cb_eye.xyz - input.position);
    
    //float3 albedoColor = pow(albedoTex.Sample(sampler0, input.texcoord0).rgb, 2.2f) * albedo.xyz;
    float3 albedoColor = albedoTex.Sample(sampler0, input.texcoord0).rgb * albedo.xyz;
    float rough = /*specularTex.Sample(sampler0, input.texcoord0).x * */roughness;
    
    Material material;
    material.diffuseColor = lerp(albedoColor, float3(0.0f, 0.0f, 0.0f), float3(metallic, metallic, metallic));
    material.specularColor = lerp(float3(0.04f, 0.04f, 0.04f), albedoColor, float3(metallic, metallic, metallic));
    material.specularRoughness = rough;

    // Lighting
    ReflectedLight reflectedLight;
    reflectedLight.directDiffuse = float3(0.0f, 0.0f, 0.0f);
    reflectedLight.directSpecular = float3(0.0f, 0.0f, 0.0f);
    reflectedLight.indirectDiffuse = float3(0.0f, 0.0f, 0.0f);
    reflectedLight.indirectSpecular = float3(0.0f, 0.0f, 0.0f);
    
    //float3 emissive = float3(0.0f, 0.0f, 0.0f);
    float3 emissive = emissiveTex.Sample(sampler0, input.texcoord0).xyz;
    float opacity = 1.0;

    IncidentLight directLight;

#if 0
    // point light
    for (int i = 0; i < LIGHT_MAX; ++i)
    {
        if (i >= numPointLights)
            break;
        getPointDirectLightIrradiance(pointLights[i], geometry, directLight);
        if (directLight.visible)
        {
            RE_Direct(directLight, geometry, material, reflectedLight);
        }
    }

    // spot light
    for (int i = 0; i < LIGHT_MAX; ++i)
    {
        if (i >= numSpotLights)
            break;
        getSpotDirectLightIrradiance(spotLights[i], geometry, directLight);
        if (directLight.visible)
        {
            RE_Direct(directLight, geometry, material, reflectedLight);
        }
    }
    
    // directional light
    for (int i=0; i<LIGHT_MAX; ++i)
    {
        if (i >= numDirectionalLights) break;
        getDirectionalDirectLightIrradiance(directionalLights[i], geometry, directLight);
        RE_Direct(directLight, geometry, material, reflectedLight);
    }
#endif
    
    // directional light
    getDirectionalDirectLightIrradiance(dirLight, geometry, directLight);
    RE_Direct(directLight, geometry, material, reflectedLight);

    float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;

    return float4(outgoingLight, opacity);
}