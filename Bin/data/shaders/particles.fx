#include "common.fx" 
 
/// -------------------- 
struct VS_FULL_OUTPUT { 
  float4 Pos   : SV_POSITION; 
  float2 UV    : TEXCOORD0; 
}; 
 
// ---------------------------------------- 
VS_FULL_OUTPUT VS_Particles( 
  float4 iPos   : POSITION,     // 0..1, 0..1, 0 en la z 
  float4 iColor : COLOR0 
  ) 
{ 
  VS_FULL_OUTPUT output = (VS_FULL_OUTPUT)0; 
  float4 pos = iPos - float4(0.5, 0.5, 0, 0); 
  float4 world_pos = mul(pos, obj_world); 
  output.Pos = mul(world_pos, camera_view_proj); 
  output.UV  = iPos.xy; 
  return output; 
} 
 
// ---------------------------------------- 
float4 PS_Particles( 
  VS_FULL_OUTPUT input 
  ) : SV_Target 
{ 
  float2 finalUV = lerp(particle_minUV, particle_maxUV, input.UV); 
  float4 oDiffuse = txAlbedo.Sample(samLinear, finalUV); 
  float4 finalColor = float4(oDiffuse.rgb * particle_color.rgb, oDiffuse.a * particle_color.a); 
  return finalColor; 
}

void VS_Particles_Mesh(
	in float4 iPos       : POSITION
	, in float3 iNormal  : NORMAL0
	, in float2 iTex0    : TEXCOORD0
	, in float2 iTex1    : TEXCOORD1
	, in float4 iTangent : NORMAL1

	, out float4 oPos    : SV_POSITION
	, out float2 oTex0   : TEXCOORD0
) {
	float4 world_pos = mul(iPos, obj_world);
	oPos = mul(world_pos, camera_view_proj);
	oTex0 = iTex0;
}

float4 PS_Particles_Mesh(
	float4 Pos     : SV_POSITION
	, float2 iTex0 : TEXCOORD0
) : SV_Target{
  float4 oDiffuse = txAlbedo.Sample(samLinear, iTex0);
  float4 finalColor = float4(oDiffuse.rgb * particle_color.rgb, oDiffuse.a * particle_color.a); 
  return finalColor; 
}
