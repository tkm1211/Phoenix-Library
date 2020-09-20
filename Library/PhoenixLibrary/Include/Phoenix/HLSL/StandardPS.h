#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer CbScene
// {
//
//   float4 cb_eye;                     // Offset:    0 Size:    16
//
// }
//
// cbuffer CbMaterial
// {
//
//   struct LightEnvironment
//   {
//       
//       struct DirLight
//       {
//           
//           float4 ambient;            // Offset:    0
//           float4 diffuse;            // Offset:   16
//           float4 specular;           // Offset:   32
//           float4 dir;                // Offset:   48
//
//       } dirLight[2];                 // Offset:    0
//       
//       struct BoxLight
//       {
//           
//           struct DirLight
//           {
//               
//               float4 ambient;        // Offset:  128
//               float4 diffuse;        // Offset:  144
//               float4 specular;       // Offset:  160
//               float4 dir;            // Offset:  176
//
//           } dir;                     // Offset:  128
//           float3 min;                // Offset:  192
//           float pad1;                // Offset:  204
//           float3 max;                // Offset:  208
//           float pad2;                // Offset:  220
//           float3 att;                // Offset:  224
//
//       } boxLight[2];                 // Offset:  128
//       
//       struct PointLight
//       {
//           
//           float3 ambient;            // Offset:  352
//           float3 diffuse;            // Offset:  368
//           float3 specular;           // Offset:  384
//           float3 att;                // Offset:  400
//           float4 pos;                // Offset:  416
//
//       } pointLight[4];               // Offset:  352
//       uint numDirLights;             // Offset:  672
//       uint numBoxLights;             // Offset:  676
//       uint numPointLights;           // Offset:  680
//       uint pad1;                     // Offset:  684
//
//   } cb_light_env;                    // Offset:    0 Size:   688
//   float4 cb_emissive;                // Offset:  688 Size:    16 [unused]
//   float4 cb_diffuse;                 // Offset:  704 Size:    16
//   float4 cb_specular;                // Offset:  720 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// sampler0                          sampler      NA          NA             s0      1 
// diffusemap                        texture  float4          2d             t0      1 
// normalmap                         texture  float4          2d             t2      1 
// CbScene                           cbuffer      NA          NA            cb0      1 
// CbMaterial                        cbuffer      NA          NA            cb1      1 
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
dcl_constantbuffer CB0[1], immediateIndexed
dcl_constantbuffer CB1[46], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t2
dcl_input_ps linear v1.xyz
dcl_input_ps linear v2.xyz
dcl_input_ps linear v3.xyz
dcl_input_ps linear v4.xy
dcl_output o0.xyzw
dcl_temps 4
dp3 r0.x, v3.xyzx, v3.xyzx
rsq r0.x, r0.x
mul r0.yzw, r0.xxxx, v3.xxyz
dp3 r1.x, v2.xyzx, v2.xyzx
rsq r1.x, r1.x
mul r1.xyz, r1.xxxx, v2.xyzx
dp3 r0.y, r1.xyzx, r0.yzwy
mad r0.xyz, v3.xyzx, r0.xxxx, -r0.yyyy
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul r0.xyz, r0.wwww, r0.xyzx
mul r2.xyz, r0.yzxy, r1.zxyz
mad r2.xyz, r1.yzxy, r0.zxyz, -r2.xyzx
dp3 r0.w, r2.xyzx, r2.xyzx
rsq r0.w, r0.w
mul r2.xyz, r0.wwww, r2.xyzx
sample_indexable(texture2d)(float,float,float,float) r3.xyz, v4.xyxx, t2.xyzw, s0
mad r3.xyz, r3.xyzx, l(2.000000, 2.000000, 2.000000, 0.000000), l(-1.000000, -1.000000, -1.000000, 0.000000)
mul r2.xyz, r2.xyzx, r3.yyyy
mad r0.xyz, r3.xxxx, r0.xyzx, r2.xyzx
mad r0.xyz, r3.zzzz, r1.xyzx, r0.xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul r0.xyz, r0.wwww, r0.xyzx
dp3 r0.w, -cb1[3].xyzx, -cb1[3].xyzx
rsq r0.w, r0.w
mul r1.xyz, r0.wwww, -cb1[3].xyzx
dp3 r0.w, -r1.xyzx, r0.xyzx
add r0.w, r0.w, r0.w
mad r2.xyz, r0.xyzx, -r0.wwww, -r1.xyzx
dp3 r0.x, r1.xyzx, r0.xyzx
max r0.x, r0.x, l(0.000000)
mad r0.x, r0.x, l(0.500000), l(0.500000)
mul r0.x, r0.x, r0.x
dp3 r0.y, r2.xyzx, r2.xyzx
rsq r0.y, r0.y
mul r0.yzw, r0.yyyy, r2.xxyz
add r1.xyz, -v1.xyzx, cb0[0].xyzx
dp3 r1.w, r1.xyzx, r1.xyzx
rsq r1.w, r1.w
mul r1.xyz, r1.wwww, r1.xyzx
dp3 r0.y, r0.yzwy, r1.xyzx
max r0.y, r0.y, l(0.000000)
log r0.y, r0.y
mul r0.y, r0.y, l(180.000000)
exp r0.y, r0.y
mul r0.y, r0.y, l(0.500000)
mul r0.yzw, r0.yyyy, cb1[45].xxyz
sample_indexable(texture2d)(float,float,float,float) r1.xyzw, v4.xyxx, t0.xyzw, s0
mul r1.xyz, r0.xxxx, r1.xyzx
mov o0.w, r1.w
mad r0.xyz, r1.xyzx, cb1[44].xyzx, r0.yzwy
mul r0.xyz, r0.xyzx, cb1[1].xyzx
mul o0.xyz, r0.xyzx, cb1[1].wwww
ret 
// Approximately 55 instruction slots used
#endif

const BYTE StandardPS[] =
{
     68,  88,  66,  67,  38,  62, 
    255, 238,  35, 234,   9, 222, 
    237,  12, 163, 108, 110,  31, 
    163, 123,   1,   0,   0,   0, 
    100,  14,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    232,   5,   0,   0, 160,   6, 
      0,   0, 212,   6,   0,   0, 
    200,  13,   0,   0,  82,  68, 
     69,  70, 172,   5,   0,   0, 
      2,   0,   0,   0,  16,   1, 
      0,   0,   5,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    255, 255,   0,   1,   0,   0, 
    131,   5,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    220,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 229,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 240,   0, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      2,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
    250,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   2,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 115,  97, 
    109, 112, 108, 101, 114,  48, 
      0, 100, 105, 102, 102, 117, 
    115, 101, 109,  97, 112,   0, 
    110, 111, 114, 109,  97, 108, 
    109,  97, 112,   0,  67,  98, 
     83,  99, 101, 110, 101,   0, 
     67,  98,  77,  97, 116, 101, 
    114, 105,  97, 108,   0, 171, 
    171, 171, 250,   0,   0,   0, 
      1,   0,   0,   0,  64,   1, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   1,   0,   0, 
      4,   0,   0,   0, 156,   1, 
      0,   0, 224,   2,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 104,   1,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
    120,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     99,  98,  95, 101, 121, 101, 
      0, 102, 108, 111,  97, 116, 
     52,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    111,   1,   0,   0,  60,   2, 
      0,   0,   0,   0,   0,   0, 
    176,   2,   0,   0,   2,   0, 
      0,   0,  60,   5,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  96,   5,   0,   0, 
    176,   2,   0,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
    120,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    108,   5,   0,   0, 192,   2, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0, 120,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 119,   5, 
      0,   0, 208,   2,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 120,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  99,  98,  95, 108, 
    105, 103, 104, 116,  95, 101, 
    110, 118,   0,  76, 105, 103, 
    104, 116,  69, 110, 118, 105, 
    114, 111, 110, 109, 101, 110, 
    116,   0, 100, 105, 114,  76, 
    105, 103, 104, 116,   0,  68, 
    105, 114,  76, 105, 103, 104, 
    116,   0,  97, 109,  98, 105, 
    101, 110, 116,   0,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    111,   1,   0,   0, 100, 105, 
    102, 102, 117, 115, 101,   0, 
    115, 112, 101,  99, 117, 108, 
     97, 114,   0, 100, 105, 114, 
      0, 171, 171, 171, 108,   2, 
      0,   0, 116,   2,   0,   0, 
      0,   0,   0,   0, 152,   2, 
      0,   0, 116,   2,   0,   0, 
     16,   0,   0,   0, 160,   2, 
      0,   0, 116,   2,   0,   0, 
     32,   0,   0,   0, 169,   2, 
      0,   0, 116,   2,   0,   0, 
     48,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,  16,   0, 
      2,   0,   4,   0, 176,   2, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     99,   2,   0,   0,  98, 111, 
    120,  76, 105, 103, 104, 116, 
      0,  66, 111, 120,  76, 105, 
    103, 104, 116,   0, 171, 171, 
      5,   0,   0,   0,   1,   0, 
     16,   0,   0,   0,   4,   0, 
    176,   2,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  99,   2,   0,   0, 
    109, 105, 110,   0, 102, 108, 
    111,  97, 116,  51,   0, 171, 
      1,   0,   3,   0,   1,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  64,   3,   0,   0, 
    112,  97, 100,  49,   0, 102, 
    108, 111,  97, 116,   0, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 113,   3,   0,   0, 
    109,  97, 120,   0, 112,  97, 
    100,  50,   0,  97, 116, 116, 
      0, 171, 171, 171, 169,   2, 
      0,   0,  24,   3,   0,   0, 
      0,   0,   0,   0,  60,   3, 
      0,   0,  72,   3,   0,   0, 
     64,   0,   0,   0, 108,   3, 
      0,   0, 120,   3,   0,   0, 
     76,   0,   0,   0, 156,   3, 
      0,   0,  72,   3,   0,   0, 
     80,   0,   0,   0, 160,   3, 
      0,   0, 120,   3,   0,   0, 
     92,   0,   0,   0, 165,   3, 
      0,   0,  72,   3,   0,   0, 
     96,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,  27,   0, 
      2,   0,   6,   0, 172,   3, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     13,   3,   0,   0, 112, 111, 
    105, 110, 116,  76, 105, 103, 
    104, 116,   0,  80, 111, 105, 
    110, 116,  76, 105, 103, 104, 
    116,   0, 112, 111, 115,   0, 
    171, 171, 108,   2,   0,   0, 
     72,   3,   0,   0,   0,   0, 
      0,   0, 152,   2,   0,   0, 
     72,   3,   0,   0,  16,   0, 
      0,   0, 160,   2,   0,   0, 
     72,   3,   0,   0,  32,   0, 
      0,   0, 165,   3,   0,   0, 
     72,   3,   0,   0,  48,   0, 
      0,   0,  46,   4,   0,   0, 
    116,   2,   0,   0,  64,   0, 
      0,   0,   5,   0,   0,   0, 
      1,   0,  16,   0,   4,   0, 
      5,   0,  52,   4,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  35,   4, 
      0,   0, 110, 117, 109,  68, 
    105, 114,  76, 105, 103, 104, 
    116, 115,   0, 100, 119, 111, 
    114, 100,   0, 171,   0,   0, 
     19,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    161,   4,   0,   0, 110, 117, 
    109,  66, 111, 120,  76, 105, 
    103, 104, 116, 115,   0, 110, 
    117, 109,  80, 111, 105, 110, 
    116,  76, 105, 103, 104, 116, 
    115,   0,  90,   2,   0,   0, 
    224,   2,   0,   0,   0,   0, 
      0,   0,   4,   3,   0,   0, 
    244,   3,   0,   0, 128,   0, 
      0,   0,  24,   4,   0,   0, 
    112,   4,   0,   0,  96,   1, 
      0,   0, 148,   4,   0,   0, 
    168,   4,   0,   0, 160,   2, 
      0,   0, 204,   4,   0,   0, 
    168,   4,   0,   0, 164,   2, 
      0,   0, 217,   4,   0,   0, 
    168,   4,   0,   0, 168,   2, 
      0,   0, 108,   3,   0,   0, 
    168,   4,   0,   0, 172,   2, 
      0,   0,   5,   0,   0,   0, 
      1,   0, 154,   0,   0,   0, 
      7,   0, 232,   4,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  73,   2, 
      0,   0,  99,  98,  95, 101, 
    109, 105, 115, 115, 105, 118, 
    101,   0,  99,  98,  95, 100, 
    105, 102, 102, 117, 115, 101, 
      0,  99,  98,  95, 115, 112, 
    101,  99, 117, 108,  97, 114, 
      0,  77, 105,  99, 114, 111, 
    115, 111, 102, 116,  32,  40, 
     82,  41,  32,  72,  76,  83, 
     76,  32,  83, 104,  97, 100, 
    101, 114,  32,  67, 111, 109, 
    112, 105, 108, 101, 114,  32, 
     49,  48,  46,  49,   0, 171, 
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
    236,   6,   0,   0,  80,   0, 
      0,   0, 187,   1,   0,   0, 
    106,   8,   0,   1,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  89,   0,   0,   4, 
     70, 142,  32,   0,   1,   0, 
      0,   0,  46,   0,   0,   0, 
     90,   0,   0,   3,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     85,  85,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      2,   0,   0,   0,  85,  85, 
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
      4,   0,   0,   0,  16,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   3,   0,   0,   0, 
     70,  18,  16,   0,   3,   0, 
      0,   0,  68,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 226,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      6,  25,  16,   0,   3,   0, 
      0,   0,  16,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  70,  18,  16,   0, 
      2,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     68,   0,   0,   5,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   1,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     16,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0, 150,   7,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   3,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  86,   5,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   2,   0,   0,   0, 
    150,   4,  16,   0,   0,   0, 
      0,   0,  38,   9,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      2,   0,   0,   0, 150,   4, 
     16,   0,   1,   0,   0,   0, 
     38,   9,  16,   0,   0,   0, 
      0,   0,  70,   2,  16, 128, 
     65,   0,   0,   0,   2,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   2,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   3,   0, 
      0,   0,  70,  16,  16,   0, 
      4,   0,   0,   0,  70, 126, 
     16,   0,   2,   0,   0,   0, 
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
    114,   0,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  86,   5, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70,   2, 
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
      0,   0,   0,   0,  16,   0, 
      0,  11, 130,   0,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  70, 130,  32, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   9, 
    114,   0,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  16,   0,   0,   8, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    114,   0,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0, 246,  15, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  70,   2, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  16,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  52,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     50,   0,   0,   9,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  63,   1,  64, 
      0,   0,   0,   0,   0,  63, 
     56,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  68,   0,   0,   5, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 226,   0,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
      6,   9,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,   9, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,  18,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     16,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
    150,   7,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  52,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  47,   0,   0,   5, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
     52,  67,  25,   0,   0,   5, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  63,  56,   0,   0,   8, 
    226,   0,  16,   0,   0,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,   6, 137, 
     32,   0,   1,   0,   0,   0, 
     45,   0,   0,   0,  69,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  16,  16,   0,   4,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 130,  32,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,  44,   0, 
      0,   0, 150,   7,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     56,   0,   0,   8, 114,  32, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0, 246, 143,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  55,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     51,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
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
