#include "common.fx"

//--------------------------------------------------------------------------------------
void VS(
	in float4 iPos     : POSITION
	, in float3 iNormal : NORMAL0
	, in float2 iTex0 : TEXCOORD0
	, in float2 iTex1 : TEXCOORD1
	, in float4 iTangent : NORMAL1

	, out float4 oPos : SV_POSITION
	, out float3 oNormal : NORMAL0
	, out float4 oTangent : NORMAL1
	, out float2 oTex0 : TEXCOORD0
	, out float2 oTex1 : TEXCOORD1
	, out float3 oWorldPos : TEXCOORD2
) {
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

float getSpecularTerm(float3 world_pos, float3 N, float3 L) {
	// Specular term (aprox)
	float3 eye = normalize(camera_pos.xyz - world_pos);
	float3 eye_r = reflect(-eye, N);
	float3 H = normalize(eye + L);
	float cos_beta = saturate(dot(eye_r, L));
	float specular_amount = pow(cos_beta, 150);
	return specular_amount;
}

//--------------------------------------------------------------------------------------
float3 Specular_F_Roughness(float3 specularColor, float gloss, float3 h, float3 v) {
	// Sclick using roughness to attenuate fresnel.
	return (specularColor + (max(gloss, specularColor) - specularColor) * pow((1 - saturate(dot(v, h))), 5));
}

float4 PS(
	float4 iPosition : SV_POSITION    // Screen coords
	, float3 iNormal : NORMAL0
	, float4 iTangent : NORMAL1
	, float2 iTex0 : TEXCOORD0
	, float2 iTex1 : TEXCOORD1
	, float3 iWorldPos : TEXCOORD2
) : SV_Target0
{

  int3 ss_load_coords = uint3(iPosition.xy, 0);

  // Convert world coords to camera space in the range 0..1 to access the rt_main texture
  float4 pos_proj_space = mul(float4(iWorldPos,1), camera_view_proj);
  float3 pos_homo_space = pos_proj_space.xyz / pos_proj_space.w;    // -1..1
  float2 pos_camera_unit_space = float2((1 + pos_homo_space.x) * 0.5, (1 - pos_homo_space.y) * 0.5);
  float4 color_base = txAlbedo.Sample(samClampLinear, pos_camera_unit_space) * obj_color.a;

  float3 eye = normalize(iWorldPos - camera_pos);

  // Apply a small fresnel effect
  float  amount = dot(iNormal, -eye);
  amount = saturate(amount);
  // Change the curve of the influence,.
  amount = pow(amount, 0.5);

  float3 irradiance_mipmaps = txEnvironmentMap.SampleLevel(samLinear, iNormal, 6).xyz;
  float3 irradiance_texture = txIrradianceMap.Sample(samLinear, iNormal).xyz;
  float3 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * (1. - scalar_irradiance_vs_mipmaps);

  float g_ReflectionIntensity = 1.0;
  float g_AmbientLightIntensity = 1.0;

  float4 self_illum = float4(0, 0, 0, 0); //txGSelfIllum.Load(uint3(iPosition.xy,0));

  float roughness = length(txRoughness.Sample(samLinear, iTex0));
  float3 env_fresnel = Specular_F_Roughness(color_base, 1. - roughness * roughness, iNormal, eye);

  float4 final_color = float4(env_fresnel * amount * g_ReflectionIntensity +
	  color_base.xyz * irradiance * g_AmbientLightIntensity
	  , 1.0f) + self_illum;

  final_color *= obj_color.a;
  return final_color;
}