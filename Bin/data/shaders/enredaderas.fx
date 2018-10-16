//--------------------------------------------------------------------------------------
#include "common.fx"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
void VS(
  in float4 iPos     : POSITION
, in float3 iNormal  : NORMAL0
, in float2 iTex0    : TEXCOORD0
, in float2 iTex1    : TEXCOORD1
, in float4 iTangent : NORMAL1

, out float4 oPos      : SV_POSITION
, out float3 oNormal   : NORMAL0
, out float4 oTangent  : NORMAL1
, out float2 oTex0     : TEXCOORD0
, out float2 oTex1     : TEXCOORD1
, out float3 oWorldPos : TEXCOORD2

)
{
  float4 world_pos = mul(iPos, obj_world);

  oPos = mul(world_pos, camera_view_proj);
  
  // Rotar la normal segun la transform del objeto
  oNormal = iNormal; // mul(iNormal, obj_world);
  oTangent.xyz = iTangent.xyz; // mul(iTangent.xyz, obj_world);
  oTangent.w = iTangent.w;

  // Las uv's se pasan directamente al ps
  oTex0 = iTex0;
  oTex1 = iTex1;
  oWorldPos = world_pos.xyz;  
}

//--------------------------------------------------------------------------------------
void PS(
    in float4 iPos      : SV_POSITION
  , in float3 iNormal   : NORMAL0
  , in float4 iTangent  : NORMAL1
  , in float2 iTex0     : TEXCOORD0
  , in float2 iTex1     : TEXCOORD1
  , in float3 iWorldPos : TEXCOORD2

  , out float4 o_albedo : SV_Target0
  , out float4 o_normal : SV_Target1
  , out float1 o_depth  : SV_Target2
  , out float4 o_self_illum : SV_Target3
)
{
  float4 texture_color = txAlbedo.Sample(samClampLinear, iTex0);

  if ( texture_color.a < 0.3 ){
    discard;
  }

  o_albedo.xyz = texture_color.xyz;
  o_albedo.a = 0.0;

  // Save roughness in the alpha coord of the N render target
  float roughness = txRoughness.Sample(samClampLinear, iTex0).r;
  o_normal = encodeNormal( iNormal, roughness );

  // Compute the Z in linear space, and normalize it in the range 0...1
  float3 camera2wpos = iWorldPos - camera_pos;
  o_depth = dot( camera_front.xyz, camera2wpos ) / camera_zfar;

  o_self_illum = txSelfIllum.Sample(samClampLinear, iTex0);
}

//--------------------------------------------------------------------------------------
// This is the PS to be used when generating the shadow map
// It's different because we can NOT use the shadow test as we are rendering to the shadow map
// and can't read and write to the same RT
float4 PS_Shadows(
    float4 iPos : SV_POSITION
  , float2 iUV : TEXCOORD0
  , float4 iColor : TEXCOORD1
  , float3 iWorldPos : TEXCOORD2
) : SV_Target
{
  float4 texture_color = txAlbedo.Sample(samLinear, iUV) * iColor;

  if ( texture_color.a < 0.3 ) 
    discard;

  return float4(1,1,1,1);
}

