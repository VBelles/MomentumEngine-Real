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

, in TInstanceWorldData instance_data     // Stream 1

, out float4 oPos      : SV_POSITION
, out float3 oNormal   : NORMAL0
, out float4 oTangent  : NORMAL1
, out float2 oTex0     : TEXCOORD0
, out float2 oTex1     : TEXCOORD1
, out float3 oWorldPos : TEXCOORD2
)
{
  float4x4 instance_world = getWorldOfInstance(instance_data);
  

  float4 world_pos = mul(iPos, instance_world);
  oPos = mul(world_pos, camera_view_proj);

  // Rotar la normal segun la transform del objeto
  oNormal = mul(iNormal, (float3x3)instance_world);
  oTangent.xyz = mul(iTangent.xyz, (float3x3)instance_world);
  oTangent.w = iTangent.w;

  // Las uv's se pasan directamente al ps
  oTex0 = iTex0;
  oTex1 = iTex1;
  oWorldPos = world_pos.xyz;
}

void VS_Vegetation(
  in float4 iPos     : POSITION
, in float3 iNormal  : NORMAL0
, in float2 iTex0    : TEXCOORD0
, in float2 iTex1    : TEXCOORD1
, in float4 iTangent : NORMAL1

, in uint   iInstancedID : SV_InstanceID  //Stream 0

, in TInstanceWorldData instance_data     // Stream 1

, out float4 oPos      : SV_POSITION
, out float3 oNormal   : NORMAL0
, out float4 oTangent  : NORMAL1
, out float2 oTex0     : TEXCOORD0
, out float2 oTex1     : TEXCOORD1
, out float3 oWorldPos : TEXCOORD2
)
{
  float4x4 instance_world = getWorldOfInstance(instance_data);

  float unit_rand_val = ( 1 + sin( iInstancedID ) ) * 0.5f;

  

  // Movimiento del viento
  //iPos.x += sin(windSpeed * unit_rand_val * global_world_time ) * range * iPos.y;
  if(iPos.y > 0){
    float windSpeed =  1.5;
    float range =  0.2;
    iPos.x += sin(windSpeed * unit_rand_val * global_world_time ) * range * iPos.y;
  }
  
  float4 world_pos = mul(iPos, instance_world);

  if(iPos.y > 0 && player_speed_length > 0 && distance(world_pos.xyz, player_position) < 0.6){ // Move when player is near
    float windSpeed =  10;
    float range =  1.2;
    iPos.x += sin(windSpeed * unit_rand_val * global_world_time ) * range * iPos.y;
    world_pos = mul(iPos, instance_world);
  }
  
  oPos = mul(world_pos, camera_view_proj);
  
  // Rotar la normal segun la transform del objeto
  oNormal = mul(iNormal, (float3x3)instance_world);
  oTangent.xyz = mul(iTangent.xyz, (float3x3)instance_world);
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
  float4 texture_color = txAlbedo.Sample(samLinear, iTex0);

  if ( texture_color.a < 0.3 ){
    discard;
  }

  o_albedo.xyz = texture_color.xyz;
  o_albedo.a = 0.0;

  // Save roughness in the alpha coord of the N render target
  float roughness = 1.0; //txRoughness.Sample(samLinear, iTex0).r;
  o_normal = encodeNormal( iNormal, roughness );

  // Compute the Z in linear space, and normalize it in the range 0...1
  float3 camera2wpos = iWorldPos - camera_pos;
  o_depth = dot( camera_front.xyz, camera2wpos ) / camera_zfar;

  o_self_illum = txSelfIllum.Sample(samLinear, iTex0);
}