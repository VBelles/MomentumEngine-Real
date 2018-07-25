#include "common.fx"

//--------------------------------------------------------------------------------------
// This shader is expected to be used only with the mesh unitQuadXY.mesh
// Where the iPos goes from 0,0..1,1
void VS(
	in float4 iPos : POSITION
	, out float4 oPos : SV_POSITION
	, out float2 oTex0 : TEXCOORD0
) {
	// Passthrough of coords and UV's
	oPos = float4(iPos.x * 2 - 1., 1 - iPos.y * 2, 0, 1);
	oTex0 = iPos.xy;
}

//--------------------------------------------------------------------------------------
float4 PS(
	in float4 iPosition : SV_Position
	, in float2 iTex0 : TEXCOORD0
) : SV_Target
{
	float4 color = txAlbedo.Sample(samClampLinear, iTex0.xy);
	float  zlinear = txGBufferLinearDepth.Load(uint3(iPosition.xy, 0)).x;
	if (zlinear == 1.0) {
		return color;
	}
	float3 wPos = getWorldCoords(iPosition.xy, zlinear);

	float distance = length(wPos.xyz - camera_pos);
	float ratio = exp(-pow(distance * fogDensity , 2));
	ratio = saturate(1 - ratio);
	ratio = ratio > fogMaxRatio ? fogMaxRatio : ratio;

  	float3 eye = wPos - camera_pos;
  	float4 env_color = txEnvironmentMap.SampleBias(samLinear, eye, 5 ) * global_ambient_adjustment;
  	//float4 env_color = txEnvironmentMap.Sample(samLinear, eye) * global_ambient_adjustment;
	//return env_color;
	return float4(lerp(color, env_color * fogColor, ratio));
}
