#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer CBShadow
// {
//
//   float4x4 lightViewProjection;      // Offset:    0 Size:    64
//   float3 shadowColor;                // Offset:   64 Size:    12 [unused]
//   float shadowBias;                  // Offset:   76 Size:     4 [unused]
//
// }
//
// cbuffer CbMaterial
// {
//
//   float4 albedo;                     // Offset:    0 Size:    16
//   float cbMetallic;                  // Offset:   16 Size:     4
//   float roughness;                   // Offset:   20 Size:     4
//   float padding01;                   // Offset:   24 Size:     4 [unused]
//   float padding02;                   // Offset:   28 Size:     4 [unused]
//
// }
//
// cbuffer CbScene
// {
//
//   struct DirectionalLight
//   {
//       
//       float4 direction;              // Offset:    0
//       float4 color;                  // Offset:   16
//
//   } dirLight[2];                     // Offset:    0 Size:    64
//   float4 cbEye;                      // Offset:   64 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// sampler0                          sampler      NA          NA             s0      1 
// albedoTex                         texture  float4          2d             t0      1 
// normalTex                         texture  float4          2d             t3      1 
// emissiveTex                       texture  float4          2d             t5      1 
// shadowMap                         texture  float4          2d             t8      1 
// CbMaterial                        cbuffer      NA          NA            cb0      1 
// CbScene                           cbuffer      NA          NA            cb1      1 
// CBShadow                          cbuffer      NA          NA            cb3      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float       
// POSITION                 0   xyz         1     NONE   float   xyz 
// NORMAL                   0   xyz         2     NONE   float   xyz 
// TANGENT                  0   xyz         3     NONE   float   xyz 
// TEXCOORD                 0   xy          4     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
ps_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB3[4], immediateIndexed
dcl_constantbuffer CB0[2], immediateIndexed
dcl_constantbuffer CB1[5], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t3
dcl_resource_texture2d (float,float,float,float) t5
dcl_resource_texture2d (float,float,float,float) t8
dcl_input_ps linear v1.xyz
dcl_input_ps linear v2.xyz
dcl_input_ps linear v3.xyz
dcl_input_ps linear v4.xy
dcl_output o0.xyzw
dcl_temps 8
mul r0.xyz, v2.yzxy, v3.zxyz
mad r0.xyz, v3.yzxy, v2.zxyz, -r0.xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul r0.xyz, r0.wwww, r0.xyzx
dp3 r0.w, v3.xyzx, v3.xyzx
rsq r0.w, r0.w
mul r1.xyz, r0.wwww, v3.xyzx
dp3 r0.w, v2.xyzx, v2.xyzx
rsq r0.w, r0.w
mul r2.xyz, r0.wwww, v2.xyzx
sample_indexable(texture2d)(float,float,float,float) r3.xyz, v4.xyxx, t3.xyzw, s0
mad r3.xyz, r3.xyzx, l(2.000000, 2.000000, 2.000000, 0.000000), l(-1.000000, -1.000000, -1.000000, 0.000000)
mul r0.xyz, r0.xyzx, r3.yyyy
mad r0.xyz, r3.xxxx, r1.xyzx, r0.xyzx
mad r0.xyz, r3.zzzz, r2.xyzx, r0.xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul r0.xyz, r0.wwww, r0.xyzx
add r1.xyz, -v1.xyzx, cb1[4].xyzx
dp3 r0.w, r1.xyzx, r1.xyzx
rsq r0.w, r0.w
mul r2.xyz, r0.wwww, r1.xyzx
sample_indexable(texture2d)(float,float,float,float) r3.xyz, v4.xyxx, t0.xyzw, s0
mul r4.xyz, r3.xyzx, cb0[0].xyzx
mad r4.xyz, cb0[1].xxxx, r4.xyzx, r4.xyzx
mad r3.xyz, r3.xyzx, cb0[0].xyzx, l(-0.040000, -0.040000, -0.040000, 0.000000)
mad r3.xyz, cb0[1].xxxx, r3.xyzx, l(0.040000, 0.040000, 0.040000, 0.000000)
sample_indexable(texture2d)(float,float,float,float) r5.xyz, v4.xyxx, t5.xyzw, s0
dp3_sat r1.w, r0.xyzx, cb1[2].xyzx
mul r6.xyz, r1.wwww, cb1[3].xyzx
mul r6.xyz, r6.xyzx, l(3.141593, 3.141593, 3.141593, 0.000000)
mul r4.xyz, r4.xyzx, l(0.318310, 0.318310, 0.318310, 0.000000)
dp3_sat r2.w, r0.xyzx, r2.xyzx
mad r1.xyz, r1.xyzx, r0.wwww, cb1[2].xyzx
dp3 r0.w, r1.xyzx, r1.xyzx
rsq r0.w, r0.w
mul r1.xyz, r0.wwww, r1.xyzx
dp3_sat r0.w, r0.xyzx, r1.xyzx
mul r3.w, cb0[1].y, cb0[1].y
mul r4.w, r3.w, r3.w
mul r0.w, r0.w, r0.w
mad r3.w, r3.w, r3.w, l(-1.000000)
mad r0.w, r0.w, r3.w, l(1.000000)
mul r0.w, r0.w, r0.w
mul r0.w, r0.w, l(3.141593)
div r0.w, r4.w, r0.w
mad r3.w, r4.w, l(0.500000), l(0.000001)
add r4.w, -r3.w, l(1.000000)
mad r5.w, r1.w, r4.w, r3.w
div r5.w, r1.w, r5.w
mad r3.w, r2.w, r4.w, r3.w
div r3.w, r2.w, r3.w
mul r3.w, r3.w, r5.w
add r7.xyz, -r3.xyzx, l(1.000000, 1.000000, 1.000000, 0.000000)
dp3_sat r1.x, r1.xyzx, r2.xyzx
add r1.x, -r1.x, l(1.000000)
mul r1.y, r1.x, r1.x
mul r1.y, r1.y, r1.y
mul r1.x, r1.y, r1.x
mad r1.xyz, r7.xyzx, r1.xxxx, r3.xyzx
mul r0.w, r0.w, r3.w
mul r1.xyz, r0.wwww, r1.xyzx
mul r0.w, r1.w, r2.w
mad r0.w, r0.w, l(4.000000), l(0.000001)
div r1.xyz, r1.xyzx, r0.wwww
dp3_sat r0.x, r0.xyzx, cb1[0].xyzx
mul r0.xyz, r0.xxxx, cb1[1].xyzx
mul r0.xyz, r4.xyzx, r0.xyzx
mul r0.xyz, r0.xyzx, l(4.712389, 4.712389, 4.712389, 0.000000)
mad r0.xyz, r6.xyzx, r4.xyzx, r0.xyzx
mov r2.xyz, v1.xyzx
mov r2.w, l(1.000000)
dp4 r3.x, r2.xyzw, cb3[0].xyzw
dp4 r3.y, r2.xyzw, cb3[1].xyzw
dp4 r3.z, r2.xyzw, cb3[2].xyzw
dp4 r0.w, r2.xyzw, cb3[3].xyzw
div r2.xyz, r3.xyzx, r0.wwww
mad r2.xy, r2.xyxx, l(0.500000, -0.500000, 0.000000, 0.000000), l(0.500000, 0.500000, 0.000000, 0.000000)
lt r3.xy, l(0.000000, 0.000000, 0.000000, 0.000000), r2.xyxx
lt r3.zw, r2.xxxy, l(0.000000, 0.000000, 1.000000, 1.000000)
and r0.w, r3.z, r3.x
and r0.w, r3.y, r0.w
and r0.w, r3.w, r0.w
if_nz r0.w
  sample_indexable(texture2d)(float,float,float,float) r0.w, r2.xyxx, t8.yzwx, s0
  add r0.w, r0.w, l(0.000500)
  lt r0.w, r0.w, r2.z
  movc r2.xyz, r0.wwww, l(0.500000,0.500000,0.500000,0), l(1.000000,1.000000,1.000000,0)
else 
  mov r2.xyz, l(1.000000,1.000000,1.000000,0)
endif 
mad r0.xyz, r0.xyzx, r2.xyzx, r5.xyzx
mad o0.xyz, r6.xyzx, r1.xyzx, r0.xyzx
mov o0.w, l(1.000000)
ret 
// Approximately 96 instruction slots used
#endif

const BYTE PhysicallyBasedRenderingSkinPS[] =
{
     68,  88,  66,  67, 168, 200, 
    220,  25,  64, 196, 198,  12, 
     34, 236, 167,  89, 122, 134, 
    129, 100,   1,   0,   0,   0, 
    140,  19,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    112,   5,   0,   0,  40,   6, 
      0,   0,  92,   6,   0,   0, 
    240,  18,   0,   0,  82,  68, 
     69,  70,  52,   5,   0,   0, 
      3,   0,   0,   0, 140,   1, 
      0,   0,   8,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    255, 255,   0,   1,   0,   0, 
     10,   5,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
     60,   1,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  69,   1,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0,  79,   1, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
     89,   1,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   5,   0,   0,   0, 
      1,   0,   0,   0,  13,   0, 
      0,   0, 101,   1,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   8,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 111,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    122,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 130,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 115,  97, 
    109, 112, 108, 101, 114,  48, 
      0,  97, 108,  98, 101, 100, 
    111,  84, 101, 120,   0, 110, 
    111, 114, 109,  97, 108,  84, 
    101, 120,   0, 101, 109, 105, 
    115, 115, 105, 118, 101,  84, 
    101, 120,   0, 115, 104,  97, 
    100, 111, 119,  77,  97, 112, 
      0,  67,  98,  77,  97, 116, 
    101, 114, 105,  97, 108,   0, 
     67,  98,  83,  99, 101, 110, 
    101,   0,  67,  66,  83, 104, 
     97, 100, 111, 119,   0, 171, 
    130,   1,   0,   0,   3,   0, 
      0,   0, 212,   1,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    111,   1,   0,   0,   5,   0, 
      0,   0,   0,   3,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    122,   1,   0,   0,   2,   0, 
      0,   0,  40,   4,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     76,   2,   0,   0,   0,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0, 108,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 144,   2, 
      0,   0,  64,   0,   0,   0, 
     12,   0,   0,   0,   0,   0, 
      0,   0, 164,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 200,   2,   0,   0, 
     76,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    220,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    108, 105, 103, 104, 116,  86, 
    105, 101, 119,  80, 114, 111, 
    106, 101,  99, 116, 105, 111, 
    110,   0, 102, 108, 111,  97, 
    116,  52, 120,  52,   0, 171, 
    171, 171,   3,   0,   3,   0, 
      4,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  96,   2, 
      0,   0, 115, 104,  97, 100, 
    111, 119,  67, 111, 108, 111, 
    114,   0, 102, 108, 111,  97, 
    116,  51,   0, 171,   1,   0, 
      3,   0,   1,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    156,   2,   0,   0, 115, 104, 
     97, 100, 111, 119,  66, 105, 
     97, 115,   0, 102, 108, 111, 
     97, 116,   0, 171, 171, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 211,   2,   0,   0, 
    200,   3,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0, 216,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 252,   3, 
      0,   0,  16,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 220,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   7,   4,   0,   0, 
     20,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    220,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     17,   4,   0,   0,  24,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 220,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  27,   4, 
      0,   0,  28,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 220,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  97, 108,  98, 101, 
    100, 111,   0, 102, 108, 111, 
     97, 116,  52,   0, 171, 171, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 207,   3,   0,   0, 
     99,  98,  77, 101, 116,  97, 
    108, 108, 105,  99,   0, 114, 
    111, 117, 103, 104, 110, 101, 
    115, 115,   0, 112,  97, 100, 
    100, 105, 110, 103,  48,  49, 
      0, 112,  97, 100, 100, 105, 
    110, 103,  48,  50,   0, 171, 
    171, 171, 120,   4,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
    224,   4,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      4,   5,   0,   0,  64,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0, 216,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 100, 105, 
    114,  76, 105, 103, 104, 116, 
      0,  68, 105, 114, 101,  99, 
    116, 105, 111, 110,  97, 108, 
     76, 105, 103, 104, 116,   0, 
    100, 105, 114, 101,  99, 116, 
    105, 111, 110,   0,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    207,   3,   0,   0,  99, 111, 
    108, 111, 114,   0, 171, 171, 
    146,   4,   0,   0, 156,   4, 
      0,   0,   0,   0,   0,   0, 
    192,   4,   0,   0, 156,   4, 
      0,   0,  16,   0,   0,   0, 
      5,   0,   0,   0,   1,   0, 
      8,   0,   2,   0,   2,   0, 
    200,   4,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 129,   4,   0,   0, 
     99,  98,  69, 121, 101,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  49, 
     48,  46,  49,   0, 171, 171, 
     73,  83,  71,  78, 176,   0, 
      0,   0,   5,   0,   0,   0, 
      8,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 140,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   7,   0,   0, 149,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   7,   0,   0, 156,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      7,   7,   0,   0, 164,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
      3,   3,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  78,  79,  82,  77,  65, 
     76,   0,  84,  65,  78,  71, 
     69,  78,  84,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0, 171, 171, 171,  79,  83, 
     71,  78,  44,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  83,  86,  95,  84, 
     97, 114, 103, 101, 116,   0, 
    171, 171,  83,  72,  69,  88, 
    140,  12,   0,   0,  80,   0, 
      0,   0,  35,   3,   0,   0, 
    106,   8,   0,   1,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      0,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   3,   0, 
      0,   0,  85,  85,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   5,   0,   0,   0, 
     85,  85,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      8,   0,   0,   0,  85,  85, 
      0,   0,  98,  16,   0,   3, 
    114,  16,  16,   0,   1,   0, 
      0,   0,  98,  16,   0,   3, 
    114,  16,  16,   0,   2,   0, 
      0,   0,  98,  16,   0,   3, 
    114,  16,  16,   0,   3,   0, 
      0,   0,  98,  16,   0,   3, 
     50,  16,  16,   0,   4,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   0,   0, 
      0,   0, 104,   0,   0,   2, 
      8,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      0,   0,   0,   0, 150,  20, 
     16,   0,   2,   0,   0,   0, 
     38,  25,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 150,  20,  16,   0, 
      3,   0,   0,   0,  38,  25, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  68,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   3,   0,   0,   0, 
     70,  18,  16,   0,   3,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   3,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      2,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   2,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   3,   0, 
      0,   0,  70,  16,  16,   0, 
      4,   0,   0,   0,  70, 126, 
     16,   0,   3,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  15, 
    114,   0,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  64, 
      0,   0,   0,  64,   0,   0, 
      0,  64,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128, 191,   0,   0, 128, 191, 
      0,   0, 128, 191,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     16,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  68,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,  18, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      2,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  69,   0,   0, 139, 
    194,   0,   0, 128,  67,  85, 
     21,   0, 114,   0,  16,   0, 
      3,   0,   0,   0,  70,  16, 
     16,   0,   4,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      4,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   4,   0,   0,   0, 
      6, 128,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      4,   0,   0,   0,  50,   0, 
      0,  13, 114,   0,  16,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,  10, 215, 
     35, 189,  10, 215,  35, 189, 
     10, 215,  35, 189,   0,   0, 
      0,   0,  50,   0,   0,  13, 
    114,   0,  16,   0,   3,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,  10, 215,  35,  61, 
     10, 215,  35,  61,  10, 215, 
     35,  61,   0,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   5,   0, 
      0,   0,  70,  16,  16,   0, 
      4,   0,   0,   0,  70, 126, 
     16,   0,   5,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  16,  32,   0,   8, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      6,   0,   0,   0, 246,  15, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
     56,   0,   0,  10, 114,   0, 
     16,   0,   6,   0,   0,   0, 
     70,   2,  16,   0,   6,   0, 
      0,   0,   2,  64,   0,   0, 
    219,  15,  73,  64, 219,  15, 
     73,  64, 219,  15,  73,  64, 
      0,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      4,   0,   0,   0,  70,   2, 
     16,   0,   4,   0,   0,   0, 
      2,  64,   0,   0, 131, 249, 
    162,  62, 131, 249, 162,  62, 
    131, 249, 162,  62,   0,   0, 
      0,   0,  16,  32,   0,   7, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  16,  32,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   9, 130,   0, 
     16,   0,   3,   0,   0,   0, 
     26, 128,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     26, 128,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     56,   0,   0,   7, 130,   0, 
     16,   0,   4,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      3,   0,   0,   0,  56,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128, 191,  50,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  56,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
    219,  15,  73,  64,  14,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   4,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  63, 
      1,  64,   0,   0, 189,  55, 
    134,  53,   0,   0,   0,   8, 
    130,   0,  16,   0,   4,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  50,   0, 
      0,   9, 130,   0,  16,   0, 
      5,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      3,   0,   0,   0,  14,   0, 
      0,   7, 130,   0,  16,   0, 
      5,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   5,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   4,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,  14,   0,   0,   7, 
    130,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
     56,   0,   0,   7, 130,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      5,   0,   0,   0,   0,   0, 
      0,  11, 114,   0,  16,   0, 
      7,   0,   0,   0,  70,   2, 
     16, 128,  65,   0,   0,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0,   0,   0, 
     16,  32,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   8,  18,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     56,   0,   0,   7,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   7,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     56,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      3,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  64,   1,  64, 
      0,   0, 189,  55, 134,  53, 
     14,   0,   0,   7, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  16,  32, 
      0,   8,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   0,   0,   8, 114,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      4,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
    228, 203, 150,  64, 228, 203, 
    150,  64, 228, 203, 150,  64, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   6,   0,   0,   0, 
     70,   2,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 114,   0,  16,   0, 
      2,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 130,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  17,   0,   0,   8, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  17,   0, 
      0,   8,  34,   0,  16,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     17,   0,   0,   8,  66,   0, 
     16,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,  17,   0,   0,   8, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,  14,   0, 
      0,   7, 114,   0,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  15, 
     50,   0,  16,   0,   2,   0, 
      0,   0,  70,   0,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0, 191,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,  63,   0,   0,   0,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  49,   0,   0,  10, 
     50,   0,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,   0, 
     16,   0,   2,   0,   0,   0, 
     49,   0,   0,  10, 194,   0, 
     16,   0,   3,   0,   0,   0, 
      6,   4,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   1,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   1,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
      1,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  31,   0, 
      4,   3,  58,   0,  16,   0, 
      0,   0,   0,   0,  69,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   2,   0, 
      0,   0, 150, 115,  16,   0, 
      8,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
    111,  18,   3,  58,  49,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,  55,   0,   0,  15, 
    114,   0,  16,   0,   2,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0,  63,   0,   0, 
      0,  63,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
      0,   0,  18,   0,   0,   1, 
     54,   0,   0,   8, 114,   0, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
      0,   0,  21,   0,   0,   1, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   5,   0,   0,   0, 
     50,   0,   0,   9, 114,  32, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   6,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,  32, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  96,   0,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
