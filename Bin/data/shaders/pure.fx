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

  //world_pos.y += /*oNormal **/ 0.005 * sin( 10 * global_world_time + world_pos.y * 0.0 + world_pos.x * 0.0 + world_pos.z * 0.0);
  oPos = mul(world_pos, camera_view_proj);

  // Las uv's se pasan directamente al ps
  oTex0 = iTex0;
  oTex1 = iTex1;
  oWorldPos = world_pos.xyz;
}

float getZViewSpace( float3 worldPos ){
	float3 delta = (worldPos - camera_pos);
	float z = dot(delta, camera_front);
	return z;
}

//--------------------------------------------------------------------------------------
float scale(float A, float A1, float A2, float Min, float Max) {
	float percentage = (A - A1) / (A1 - A2);
	return (percentage) * (Min - Max) + Min;
}


float4 PS(
  float4 iPosition : SV_POSITION    // Screen coords
, float3 iNormal   : NORMAL0
, float4 iTangent  : NORMAL1
, float2 iTex0     : TEXCOORD0
, float2 iTex1     : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
)  : SV_Target0
{
	// Obtain a random value associated to each pos in the surface
	float4 noise0 = txNoiseMap.Sample( samLinear, iTex0 * 2.0 + 0.2 * global_world_time * float2(.5,0)) * 2 - 1;      // -1..1
	float4 noise1 = txNoiseMap.Sample( samLinear, iTex0 * 8.0 + 0.1 * global_world_time * float2(.5,0.1)) * 2 - 1;      // -1..1
	float4 noise2 = txNoiseMap.Sample( samLinear, iTex0 * 16 + 0.1 * global_world_time * float2(.55,-0.123)) * 2 - 1;      // -1..1

	// Add 3 octaves to break pattern repetition
	float2 noiseF = noise0.xy + noise1.xy * 0.5 + noise2.xy * .25;

	float3 normal = computeNormalMap(iTex0, computeTBN(iNormal, iTangent));
	float3 noise = normal * 0.01f;

	float4 albedo = txAlbedo.Sample(samClampLinear, iTex0);
	albedo.rgb = saturate(albedo.rgb * 1.7);
	albedo.r += 0.2 * sin(2 * global_world_time + iNormal.x  + iNormal.z ) + 0.3 * sin(noiseF.x);
	albedo.g += 0.2 * sin(2 * global_world_time + iNormal.x  + iNormal.z ) + 0.3 * sin(noiseF.x);
	albedo.b += 0.2 * sin(2 * global_world_time + iNormal.x  + iNormal.z ) + 0.3 * sin(noiseF.x);

	float4 pos_proj_space = mul(float4(iWorldPos, 1), camera_view_proj);
	float3 pos_homo_space = pos_proj_space.xyz / pos_proj_space.w;    // -1..1
	float2 pos_camera_unit_space = float2(
		(1 + pos_homo_space.x) * 0.5 + 0.0005 * (1 - sin( 3 * global_world_time + iWorldPos.x + iWorldPos.z + iWorldPos.y)) + 0.05 * noiseF.x, 
		(1 - pos_homo_space.y) * 0.5 + 0.0005 * (1 - sin( 5 * global_world_time + iWorldPos.x + iWorldPos.z + iWorldPos.y)) + 0.05 * noiseF.y
		);

	float4 background_color;
	float zlinear = txGBufferLinearDepth.Sample(samClampPoint, pos_camera_unit_space + noise.xy).x;
	if (zlinear * camera_zfar > getZViewSpace(iWorldPos)) {
		background_color = txGBufferAlbedos.Sample(samClampPoint, pos_camera_unit_space + noise.xy);	
	}
	else {
		background_color = txGBufferAlbedos.Sample(samClampLinear, pos_camera_unit_space);
	}

	float4 irradiance_mipmaps = float4(getEnvironment(normal, 6), 1);
	float4 irradiance_texture = float4(getIrradiance(normal), 1);
	float4 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * (1. - scalar_irradiance_vs_mipmaps);

	float4 final_color = lerp(irradiance, background_color, 0.75f);
	return lerp(final_color, albedo, 0.3);
}
