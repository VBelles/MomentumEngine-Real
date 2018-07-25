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

  int3 ss_load_coords = uint3(input.Pos.xy, 0);
  float zlinear = txGBufferLinearDepth.Load(ss_load_coords).x;

  float3 camera2wpos = input.Pos - camera_pos;
  float particleDepth = dot(camera_front.xyz, camera2wpos) / camera_zfar;

  float depthDiff = zlinear - particleDepth;

  //return float4(zlinear * 100, zlinear * 100, zlinear * 100, 1);
  //return float4(depthDiff * 100, depthDiff * 100, depthDiff * 100, 1);
  //return float4(particleDepth * 100, particleDepth * 100, particleDepth * 100, 1);

  //if(depthDiff <= 0 ){
  //  discard;
  //}
    
  //return float4(zlinear, zlinear, zlinear, 1);

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
