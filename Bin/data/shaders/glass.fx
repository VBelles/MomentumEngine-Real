#include "common.fx"

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

  // Rotar la normal segun la transform del objeto
  oNormal = mul(iNormal, (float3x3)obj_world);
  oTangent.xyz = mul(iTangent.xyz, (float3x3)obj_world);
  oTangent.w = iTangent.w;

  oPos = mul(world_pos, camera_view_proj);

  // Las uv's se pasan directamente al ps
  oTex0 = iTex0;
  oTex1 = iTex1;
  oWorldPos = world_pos.xyz;
}

//--------------------------------------------------------------------------------------
float4 PS(
  float4 iPosition : SV_POSITION    // Screen coords
, float3 iNormal   : NORMAL0
, float4 iTangent  : NORMAL1
, float2 iTex0     : TEXCOORD0
, float2 iTex1     : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
)  : SV_Target0
{
	float3 normal = computeNormalMap(iNormal, iTangent, iTex0);
	float3 noise = normal * 0.01f;

	float4 albedo = txAlbedo.Sample(samClampLinear, iTex0);

	float4 pos_proj_space = mul(float4(iWorldPos, 1), camera_view_proj);
	float3 pos_homo_space = pos_proj_space.xyz / pos_proj_space.w;    // -1..1
	float2 pos_camera_unit_space = float2((1 + pos_homo_space.x) * 0.5, (1 - pos_homo_space.y) * 0.5);

	float4 background_color;
	float zlinear = txGBufferLinearDepth.Sample(samLinear, pos_camera_unit_space + noise.xy).x;

	if (zlinear > (1 - pos_homo_space.z) * 0.5f) {
		background_color = txGBufferAlbedos.Sample(samLinear, pos_camera_unit_space + noise.xy);
	}
	else {
		background_color = txGBufferAlbedos.Sample(samLinear, pos_camera_unit_space);
	}

	float4 irradiance_mipmaps = txEnvironmentMap.SampleLevel(samLinear, normal, 6);
	float4 irradiance_texture = txIrradianceMap.Sample(samLinear, normal);
	float4 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * (1. - scalar_irradiance_vs_mipmaps);

	float4 final_color = lerp(irradiance, background_color, 0.75f);
	return lerp(final_color, albedo, albedo.a);
}
