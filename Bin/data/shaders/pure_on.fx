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
	oPos = mul(world_pos, camera_view_proj);

	// Rotar la normal segun la transform del objeto
	oNormal = mul(iNormal, (float3x3)obj_world);
	oTangent.xyz = mul(iTangent.xyz, (float3x3)obj_world);
	oTangent.w = iTangent.w;

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


void PS(
	float4 Pos       : SV_POSITION
	, float3 iNormal : NORMAL0
	, float4 iTangent : NORMAL1
	, float2 iTex0 : TEXCOORD0
	, float2 iTex1 : TEXCOORD1
	, float3 iWorldPos : TEXCOORD2
	, out float4 o_albedo : SV_Target0
	, out float4 o_normal : SV_Target1
	, out float1 o_depth : SV_Target2
	, out float4 o_selfIllum : SV_Target3
) {
	// Obtain a random value associated to each pos in the surface
	float4 noise0 = txNoiseMap.Sample( samLinear, iTex0 * 2.0 + 0.2 * global_world_time * float2(.5,0)) * 2 - 1;      // -1..1
	float4 noise1 = txNoiseMap.Sample( samLinear, iTex0 * 8.0 + 0.1 * global_world_time * float2(.5,0.1)) * 2 - 1;      // -1..1
	float4 noise2 = txNoiseMap.Sample( samLinear, iTex0 * 16 + 0.1 * global_world_time * float2(.55,-0.123)) * 2 - 1;      // -1..1

	// Add 3 octaves to break pattern repetition
	float2 noiseF = noise0.xy + noise1.xy * 0.5 + noise2.xy * .25;
	float3 camera2wpos = iWorldPos - camera_pos;

	// Store in the Alpha channel of the albedo texture, the 'metallic' amount of
	// the material
	o_albedo = txAlbedo.Sample(samLinear, iTex0);
	o_albedo.a = txMetallic.Sample(samLinear, iTex0).r;

	float3 N = computeNormalMap(iNormal, iTangent, iTex0);

	// Save roughness in the alpha coord of the N render target
	float roughness = txRoughness.Sample(samLinear, iTex0).r;
	o_normal = encodeNormal(N, roughness);

	o_selfIllum = txSelfIllum.Sample(samLinear, iTex0);

	float red = saturate	(-0.1 + 0.25 * sin(3.0 * global_world_time));
	float green = saturate	(-0.1 + 0.25 * sin(3.0 * global_world_time));
	float blue = saturate	(-0.1 + 0.25 * sin(3.0 * global_world_time));
	float redThreshold 		= 0.13;
	float greenThreshold 	= 0.13;
	float blueThreshold 	= 0.14;
	if(red > redThreshold) red = redThreshold;
	if(green > greenThreshold) green = greenThreshold;
	if(blue > blueThreshold) blue = blueThreshold;
	o_selfIllum = float4(
		red,
		green,
		blue,
		1);

	// Compute the Z in linear space, and normalize it in the range 0...1
	// In the range z=0 to z=zFar of the camera (not zNear)
	o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}
