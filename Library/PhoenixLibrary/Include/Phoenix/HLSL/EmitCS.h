#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer EmittedParticleCB
// {
//
//   float4x4 xEmitterWorld;            // Offset:    0 Size:    64
//   uint xEmitCount;                   // Offset:   64 Size:     4 [unused]
//   uint xEmitterMeshIndexCount;       // Offset:   68 Size:     4 [unused]
//   uint xEmitterMeshVertexPositionStride;// Offset:   72 Size:     4 [unused]
//   float xEmitterRandomness;          // Offset:   76 Size:     4
//   float xParticleSize;               // Offset:   80 Size:     4
//   float xParticleScaling;            // Offset:   84 Size:     4
//   float xParticleRotation;           // Offset:   88 Size:     4
//   uint xParticleColor;               // Offset:   92 Size:     4
//   float4 xParticleNormal;            // Offset:   96 Size:    16
//   float4 xParticleMainColor;         // Offset:  112 Size:    16 [unused]
//   float xParticleRandomFactor;       // Offset:  128 Size:     4
//   float xParticleNormalFactor;       // Offset:  132 Size:     4
//   float xParticleLifeSpan;           // Offset:  136 Size:     4
//   float xParticleLifeSpanRandomness; // Offset:  140 Size:     4
//   float xParticleMass;               // Offset:  144 Size:     4
//   float xParticleMotionBlurAmount;   // Offset:  148 Size:     4 [unused]
//   float xEmitterOpacity;             // Offset:  152 Size:     4 [unused]
//   uint xEmitterMaxParticleCount;     // Offset:  156 Size:     4 [unused]
//   float xSPH_h;                      // Offset:  160 Size:     4 [unused]
//   float xSPH_h_rcp;                  // Offset:  164 Size:     4 [unused]
//   float xSPH_h2;                     // Offset:  168 Size:     4 [unused]
//   float xSPH_h3;                     // Offset:  172 Size:     4 [unused]
//   float xSPH_poly6_constant;         // Offset:  176 Size:     4 [unused]
//   float xSPH_spiky_constant;         // Offset:  180 Size:     4 [unused]
//   float xSPH_K;                      // Offset:  184 Size:     4 [unused]
//   float xSPH_p0;                     // Offset:  188 Size:     4 [unused]
//   float xSPH_e;                      // Offset:  192 Size:     4 [unused]
//   uint xSPH_ENABLED;                 // Offset:  196 Size:     4 [unused]
//   float xEmitterFixedTimestep;       // Offset:  200 Size:     4 [unused]
//   float xParticleEmissive;           // Offset:  204 Size:     4 [unused]
//   float xSeed;                       // Offset:  208 Size:     4 [unused]
//   float randU;                       // Offset:  212 Size:     4 [unused]
//   float randV;                       // Offset:  216 Size:     4 [unused]
//   float randW;                       // Offset:  220 Size:     4 [unused]
//
// }
//
// cbuffer FrameTimeCB
// {
//
//   float frameTime;                   // Offset:    0 Size:     4
//   float3 dummy;                      // Offset:    4 Size:    12 [unused]
//
// }
//
// Resource bind info for particleBuffer
// {
//
//   struct Particle
//   {
//       
//       float3 position;               // Offset:    0
//       float mass;                    // Offset:   12
//       float3 force;                  // Offset:   16
//       float rotationalVelocity;      // Offset:   28
//       float3 velocity;               // Offset:   32
//       float maxLife;                 // Offset:   44
//       float2 sizeBeginEnd;           // Offset:   48
//       float life;                    // Offset:   56
//       uint colorMirror;              // Offset:   60
//
//   } $Element;                        // Offset:    0 Size:    64
//
// }
//
// Resource bind info for aliveBuffer_CURRENT
// {
//
//   uint $Element;                     // Offset:    0 Size:     4
//
// }
//
// Resource bind info for deadBuffer
// {
//
//   uint $Element;                     // Offset:    0 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// particleBuffer                        UAV  struct         r/w             u0      1 
// aliveBuffer_CURRENT                   UAV  struct         r/w             u1      1 
// deadBuffer                            UAV  struct         r/w             u3      1 
// counterBuffer                         UAV    byte         r/w             u4      1 
// EmittedParticleCB                 cbuffer      NA          NA            cb0      1 
// FrameTimeCB                       cbuffer      NA          NA            cb1      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[10], immediateIndexed
dcl_constantbuffer CB1[1], immediateIndexed
dcl_uav_structured u0, 64
dcl_uav_structured u1, 4
dcl_uav_structured u3, 4
dcl_uav_raw u4
dcl_input vThreadID.x
dcl_temps 5
dcl_thread_group 256, 1, 1
ld_raw_indexable(raw_buffer)(mixed,mixed,mixed,mixed) r0.x, l(8), u4.xxxx
ult r0.x, vThreadID.x, r0.x
if_nz r0.x
  add r0.x, cb0[4].w, cb1[0].x
  utof r0.z, vThreadID.x
  mul r0.y, r0.z, l(0.003906)
  dp2 r0.x, r0.xyxx, l(12.989800, 78.233002, 0.000000, 0.000000)
  mul r1.xyzw, r0.xxxx, l(0.123450, 1.123450, 2.123450, 3.123450)
  sincos r1.xyzw, null, r1.xyzw
  mul r1.xyzw, r1.xyzw, l(43758.546875, 43758.546875, 43758.546875, 43758.546875)
  frc r1.xyzw, r1.xyzw
  add r1.xyzw, r1.xyzw, l(-0.500000, -0.500000, -0.500000, -0.500000)
  mul r0.y, r1.x, cb0[5].x
  mad r2.x, r0.y, cb0[8].x, cb0[5].x
  mad r0.yzw, r1.yyzw, cb0[8].xxxx, cb0[6].xxyz
  mul r1.xyz, r0.yzwy, cb0[8].yyyy
  mul r0.xy, r0.xxxx, l(4.123450, 5.123450, 0.000000, 0.000000)
  sincos r0.xy, null, r0.xyxx
  mul r0.xy, r0.xyxx, l(43758.546875, 43758.546875, 0.000000, 0.000000)
  frc r0.xy, r0.xyxx
  add r0.xy, r0.xyxx, l(-0.500000, -0.500000, 0.000000, 0.000000)
  mul r0.y, r0.y, cb0[8].z
  mad r3.w, r0.x, cb0[8].x, cb0[5].z
  mad r1.w, r0.y, cb0[8].w, cb0[8].z
  mul r2.y, r2.x, cb0[5].y
  and r2.w, cb0[5].w, l(0x00ffffff)
  imm_atomic_iadd r0.x, u4, l(4), l(-1)
  iadd r0.x, r0.x, l(-1)
  ld_structured_indexable(structured_buffer, stride=4)(mixed,mixed,mixed,mixed) r0.x, r0.x, l(0), u3.xxxx
  mov r4.xyz, cb0[3].xyzx
  mov r4.w, cb0[9].x
  store_structured u0.xyzw, r0.x, l(0), r4.xyzw
  mov r3.xyz, l(0,0,0,0)
  store_structured u0.xyzw, r0.x, l(16), r3.xyzw
  store_structured u0.xyzw, r0.x, l(32), r1.xyzw
  mov r2.z, r1.w
  store_structured u0.xyzw, r0.x, l(48), r2.xyzw
  imm_atomic_iadd r1.x, u4, l(0), l(1)
  store_structured u1.x, r1.x, l(0), r0.x
endif 
ret 
// Approximately 41 instruction slots used
#endif

const BYTE EmitCS[] =
{
     68,  88,  66,  67, 248,  48, 
     54,   9, 156,  61, 184, 148, 
    208, 109,   8, 212,  84, 107, 
     50, 218,   1,   0,   0,   0, 
    112,  19,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     28,  13,   0,   0,  44,  13, 
      0,   0,  60,  13,   0,   0, 
    212,  18,   0,   0,  82,  68, 
     69,  70, 224,  12,   0,   0, 
      5,   0,   0,   0,  88,   1, 
      0,   0,   6,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
     83,  67,   0,   1,   0,   0, 
    184,  12,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    252,   0,   0,   0,   6,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  11,   1,   0,   0, 
      6,   0,   0,   0,   6,   0, 
      0,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  31,   1, 
      0,   0,   6,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,   4,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     42,   1,   0,   0,   8,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  56,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  74,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    112,  97, 114, 116, 105,  99, 
    108, 101,  66, 117, 102, 102, 
    101, 114,   0,  97, 108, 105, 
    118, 101,  66, 117, 102, 102, 
    101, 114,  95,  67,  85,  82, 
     82,  69,  78,  84,   0, 100, 
    101,  97, 100,  66, 117, 102, 
    102, 101, 114,   0,  99, 111, 
    117, 110, 116, 101, 114,  66, 
    117, 102, 102, 101, 114,   0, 
     69, 109, 105, 116, 116, 101, 
    100,  80,  97, 114, 116, 105, 
     99, 108, 101,  67,  66,   0, 
     70, 114,  97, 109, 101,  84, 
    105, 109, 101,  67,  66,   0, 
    171, 171,  56,   1,   0,   0, 
     35,   0,   0,   0, 208,   1, 
      0,   0, 224,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  74,   1,   0,   0, 
      2,   0,   0,   0,  28,  10, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 252,   0,   0,   0, 
      1,   0,   0,   0, 168,  10, 
      0,   0,  64,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  11,   1,   0,   0, 
      1,   0,   0,   0, 104,  12, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  31,   1,   0,   0, 
      1,   0,   0,   0, 144,  12, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  72,   7,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
     96,   7,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    132,   7,   0,   0,  64,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 152,   7, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 188,   7, 
      0,   0,  68,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 152,   7,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 211,   7,   0,   0, 
     72,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    152,   7,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    244,   7,   0,   0,  76,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  52,   8, 
      0,   0,  80,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  66,   8,   0,   0, 
     84,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     83,   8,   0,   0,  88,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 101,   8, 
      0,   0,  92,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 152,   7,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 116,   8,   0,   0, 
     96,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
    140,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    176,   8,   0,   0, 112,   0, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0, 140,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 195,   8, 
      0,   0, 128,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 217,   8,   0,   0, 
    132,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    239,   8,   0,   0, 136,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   9, 
      0,   0, 140,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  29,   9,   0,   0, 
    144,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     43,   9,   0,   0, 148,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  69,   9, 
      0,   0, 152,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  85,   9,   0,   0, 
    156,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    152,   7,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    110,   9,   0,   0, 160,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 117,   9, 
      0,   0, 164,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 128,   9,   0,   0, 
    168,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    136,   9,   0,   0, 172,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 144,   9, 
      0,   0, 176,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 164,   9,   0,   0, 
    180,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    184,   9,   0,   0, 184,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 191,   9, 
      0,   0, 188,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 199,   9,   0,   0, 
    192,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    206,   9,   0,   0, 196,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 152,   7, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 219,   9, 
      0,   0, 200,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 241,   9,   0,   0, 
    204,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      3,  10,   0,   0, 208,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   9,  10, 
      0,   0, 212,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  16,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  15,  10,   0,   0, 
    216,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     21,  10,   0,   0, 220,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  16,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 120,  69, 
    109, 105, 116, 116, 101, 114, 
     87, 111, 114, 108, 100,   0, 
    102, 108, 111,  97, 116,  52, 
    120,  52,   0, 171,   3,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     86,   7,   0,   0, 120,  69, 
    109, 105, 116,  67, 111, 117, 
    110, 116,   0, 100, 119, 111, 
    114, 100,   0, 171, 171, 171, 
      0,   0,  19,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 143,   7,   0,   0, 
    120,  69, 109, 105, 116, 116, 
    101, 114,  77, 101, 115, 104, 
     73, 110, 100, 101, 120,  67, 
    111, 117, 110, 116,   0, 120, 
     69, 109, 105, 116, 116, 101, 
    114,  77, 101, 115, 104,  86, 
    101, 114, 116, 101, 120,  80, 
    111, 115, 105, 116, 105, 111, 
    110,  83, 116, 114, 105, 100, 
    101,   0, 120,  69, 109, 105, 
    116, 116, 101, 114,  82,  97, 
    110, 100, 111, 109, 110, 101, 
    115, 115,   0, 102, 108, 111, 
     97, 116,   0, 171, 171, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   7,   8,   0,   0, 
    120,  80,  97, 114, 116, 105, 
     99, 108, 101,  83, 105, 122, 
    101,   0, 120,  80,  97, 114, 
    116, 105,  99, 108, 101,  83, 
     99,  97, 108, 105, 110, 103, 
      0, 120,  80,  97, 114, 116, 
    105,  99, 108, 101,  82, 111, 
    116,  97, 116, 105, 111, 110, 
      0, 120,  80,  97, 114, 116, 
    105,  99, 108, 101,  67, 111, 
    108, 111, 114,   0, 120,  80, 
     97, 114, 116, 105,  99, 108, 
    101,  78, 111, 114, 109,  97, 
    108,   0, 102, 108, 111,  97, 
    116,  52,   0, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    132,   8,   0,   0, 120,  80, 
     97, 114, 116, 105,  99, 108, 
    101,  77,  97, 105, 110,  67, 
    111, 108, 111, 114,   0, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  82,  97, 110, 100, 
    111, 109,  70,  97,  99, 116, 
    111, 114,   0, 120,  80,  97, 
    114, 116, 105,  99, 108, 101, 
     78, 111, 114, 109,  97, 108, 
     70,  97,  99, 116, 111, 114, 
      0, 120,  80,  97, 114, 116, 
    105,  99, 108, 101,  76, 105, 
    102, 101,  83, 112,  97, 110, 
      0, 120,  80,  97, 114, 116, 
    105,  99, 108, 101,  76, 105, 
    102, 101,  83, 112,  97, 110, 
     82,  97, 110, 100, 111, 109, 
    110, 101, 115, 115,   0, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  77,  97, 115, 115, 
      0, 120,  80,  97, 114, 116, 
    105,  99, 108, 101,  77, 111, 
    116, 105, 111, 110,  66, 108, 
    117, 114,  65, 109, 111, 117, 
    110, 116,   0, 120,  69, 109, 
    105, 116, 116, 101, 114,  79, 
    112,  97,  99, 105, 116, 121, 
      0, 120,  69, 109, 105, 116, 
    116, 101, 114,  77,  97, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  67, 111, 117, 110, 
    116,   0, 120,  83,  80,  72, 
     95, 104,   0, 120,  83,  80, 
     72,  95, 104,  95, 114,  99, 
    112,   0, 120,  83,  80,  72, 
     95, 104,  50,   0, 120,  83, 
     80,  72,  95, 104,  51,   0, 
    120,  83,  80,  72,  95, 112, 
    111, 108, 121,  54,  95,  99, 
    111, 110, 115, 116,  97, 110, 
    116,   0, 120,  83,  80,  72, 
     95, 115, 112, 105, 107, 121, 
     95,  99, 111, 110, 115, 116, 
     97, 110, 116,   0, 120,  83, 
     80,  72,  95,  75,   0, 120, 
     83,  80,  72,  95, 112,  48, 
      0, 120,  83,  80,  72,  95, 
    101,   0, 120,  83,  80,  72, 
     95,  69,  78,  65,  66,  76, 
     69,  68,   0, 120,  69, 109, 
    105, 116, 116, 101, 114,  70, 
    105, 120, 101, 100,  84, 105, 
    109, 101, 115, 116, 101, 112, 
      0, 120,  80,  97, 114, 116, 
    105,  99, 108, 101,  69, 109, 
    105, 115, 115, 105, 118, 101, 
      0, 120,  83, 101, 101, 100, 
      0, 114,  97, 110, 100,  85, 
      0, 114,  97, 110, 100,  86, 
      0, 114,  97, 110, 100,  87, 
      0, 171, 108,  10,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    118,  10,   0,   0,   4,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0, 132,  10, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 102, 114, 
     97, 109, 101,  84, 105, 109, 
    101,   0, 100, 117, 109, 109, 
    121,   0, 102, 108, 111,  97, 
    116,  51,   0, 171,   1,   0, 
      3,   0,   1,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    124,  10,   0,   0, 208,  10, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,  68,  12,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  36,  69, 108, 101, 
    109, 101, 110, 116,   0,  80, 
     97, 114, 116, 105,  99, 108, 
    101,   0, 112, 111, 115, 105, 
    116, 105, 111, 110,   0, 171, 
      1,   0,   3,   0,   1,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 124,  10,   0,   0, 
    109,  97, 115, 115,   0, 171, 
    171, 171,   0,   0,   3,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   7,   8, 
      0,   0, 102, 111, 114,  99, 
    101,   0, 114, 111, 116,  97, 
    116, 105, 111, 110,  97, 108, 
     86, 101, 108, 111,  99, 105, 
    116, 121,   0, 118, 101, 108, 
    111,  99, 105, 116, 121,   0, 
    109,  97, 120,  76, 105, 102, 
    101,   0, 115, 105, 122, 101, 
     66, 101, 103, 105, 110,  69, 
    110, 100,   0, 102, 108, 111, 
     97, 116,  50,   0, 171, 171, 
      1,   0,   3,   0,   1,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 115,  11,   0,   0, 
    108, 105, 102, 101,   0,  99, 
    111, 108, 111, 114,  77, 105, 
    114, 114, 111, 114,   0, 171, 
    171, 171,   0,   0,  19,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 143,   7, 
      0,   0, 226,  10,   0,   0, 
    236,  10,   0,   0,   0,   0, 
      0,   0,  16,  11,   0,   0, 
     24,  11,   0,   0,  12,   0, 
      0,   0,  60,  11,   0,   0, 
    236,  10,   0,   0,  16,   0, 
      0,   0,  66,  11,   0,   0, 
     24,  11,   0,   0,  28,   0, 
      0,   0,  85,  11,   0,   0, 
    236,  10,   0,   0,  32,   0, 
      0,   0,  94,  11,   0,   0, 
     24,  11,   0,   0,  44,   0, 
      0,   0, 102,  11,   0,   0, 
    124,  11,   0,   0,  48,   0, 
      0,   0, 160,  11,   0,   0, 
     24,  11,   0,   0,  56,   0, 
      0,   0, 165,  11,   0,   0, 
    180,  11,   0,   0,  60,   0, 
      0,   0,   5,   0,   0,   0, 
      1,   0,  16,   0,   0,   0, 
      9,   0, 216,  11,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 217,  10, 
      0,   0, 208,  10,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    180,  11,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    208,  10,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 180,  11, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0,  73,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     79,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  83,  72, 
     69,  88, 144,   5,   0,   0, 
     80,   0,   5,   0, 100,   1, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 158,   0,   0,   4, 
      0, 224,  17,   0,   0,   0, 
      0,   0,  64,   0,   0,   0, 
    158,   0,   0,   4,   0, 224, 
     17,   0,   1,   0,   0,   0, 
      4,   0,   0,   0, 158,   0, 
      0,   4,   0, 224,  17,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0, 157,   0,   0,   3, 
      0, 224,  17,   0,   4,   0, 
      0,   0,  95,   0,   0,   2, 
     18,   0,   2,   0, 104,   0, 
      0,   2,   5,   0,   0,   0, 
    155,   0,   0,   4,   0,   1, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 165,   0, 
      0, 137, 194,   2,   0, 128, 
    131, 153,  25,   0,  18,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   8,   0, 
      0,   0,   6, 224,  17,   0, 
      4,   0,   0,   0,  79,   0, 
      0,   6,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
      2,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  31,   0, 
      4,   3,  10,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   9,  18,   0,  16,   0, 
      0,   0,   0,   0,  58, 128, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  10, 128, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  86,   0, 
      0,   4,  66,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
      2,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  59, 
     15,   0,   0,  10,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
     57, 214,  79,  65,  76, 119, 
    156,  66,   0,   0,   0,   0, 
      0,   0,   0,   0,  56,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,  91, 211, 
    252,  61,  54, 205, 143,  63, 
    155, 230,   7,  64, 155, 230, 
     71,  64,  77,   0,   0,   6, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   0, 208,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0, 140, 238,  42,  71, 
    140, 238,  42,  71, 140, 238, 
     42,  71, 140, 238,  42,  71, 
     26,   0,   0,   5, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0, 191, 
      0,   0,   0, 191,   0,   0, 
      0, 191,   0,   0,   0, 191, 
     56,   0,   0,   8,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  10, 128,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  11, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  11, 226,   0,  16,   0, 
      0,   0,   0,   0,  86,  14, 
     16,   0,   1,   0,   0,   0, 
      6, 128,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
      6, 137,  32,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
     56,   0,   0,   8, 114,   0, 
     16,   0,   1,   0,   0,   0, 
    150,   7,  16,   0,   0,   0, 
      0,   0,  86, 133,  32,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  56,   0,   0,  10, 
     50,   0,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,  78, 243, 131,  64, 
     78, 243, 163,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     77,   0,   0,   6,  50,   0, 
     16,   0,   0,   0,   0,   0, 
      0, 208,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10,  50,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
    140, 238,  42,  71, 140, 238, 
     42,  71,   0,   0,   0,   0, 
      0,   0,   0,   0,  26,   0, 
      0,   5,  50,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,  10,  50,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0, 191,   0,   0, 
      0, 191,   0,   0,   0,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     42, 128,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     50,   0,   0,  11, 130,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  10, 128,  32,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  42, 128,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  11, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  58, 128, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  42, 128, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  56,   0, 
      0,   8,  34,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     26, 128,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
      1,   0,   0,   8, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     58, 128,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
      1,  64,   0,   0, 255, 255, 
    255,   0, 180,   0,   0,   9, 
     18,   0,  16,   0,   0,   0, 
      0,   0,   0, 224,  17,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   4,   0,   0,   0, 
      1,  64,   0,   0, 255, 255, 
    255, 255,  30,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 255, 255, 255, 255, 
    167,   0,   0, 139,   2,  35, 
      0, 128, 131, 153,  25,   0, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 224,  17,   0,   3,   0, 
      0,   0,  54,   0,   0,   6, 
    114,   0,  16,   0,   4,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  54,   0,   0,   6, 
    130,   0,  16,   0,   4,   0, 
      0,   0,  10, 128,  32,   0, 
      0,   0,   0,   0,   9,   0, 
      0,   0, 168,   0,   0,   9, 
    242, 224,  17,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0,  54,   0,   0,   8, 
    114,   0,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 168,   0, 
      0,   9, 242, 224,  17,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0, 168,   0, 
      0,   9, 242, 224,  17,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  32,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5,  66,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
    168,   0,   0,   9, 242, 224, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     48,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
    180,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
      0, 224,  17,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
    168,   0,   0,   9,  18, 224, 
     17,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  41,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  21,   0,   0,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0
};
