#include "common.fx" 
 
/// -------------------- 
struct VS_FULL_OUTPUT { 
  float4 Pos   : SV_POSITION; 
  float2 UV    : TEXCOORD0; 
  float3 worldPos : TEXCOORD1;
}; 


// ---------------------------------------- 
VS_FULL_OUTPUT VS_Particles( 
  float4 iPos   : POSITION,     // 0..1, 0..1, 0 en la z 
  float4 iColor : COLOR0 
  ) 
{ 
  VS_FULL_OUTPUT output = (VS_FULL_OUTPUT)0;
  output.UV  = iPos.xy;
  iPos -= float4(0.5, 0.5, 0, 0);
  float4 world_pos = mul(iPos, obj_world);
  output.Pos = mul(world_pos, camera_view_proj);
  output.worldPos = world_pos;
  return output; 
}
 
// ---------------------------------------- 
VS_FULL_OUTPUT VS_Stretched_Particles( 
  float4 iPos   : POSITION,     // 0..1, 0..1, 0 en la z 
  float4 iColor : COLOR0 
  ) 
{ 

  VS_FULL_OUTPUT output = (VS_FULL_OUTPUT)0;
  output.UV  = iPos.xy;
  iPos -= float4(0.5, 0.5, 0, 0);

  float3 quadPos = iPos.xyz;

  // Rotate the billboard
	float2x2 rot = float2x2(
		cos(particle_rotation), -sin(particle_rotation),
		sin(particle_rotation), cos(particle_rotation)
		);
	quadPos.xy = mul(quadPos.xy, rot);

  // Scale the billboard
  quadPos *= particle_scale;

  // Scale the billboard along view space motion vector
  float3 velocity = mul(particle_velocity, (float3x3)camera_view);
	quadPos += dot(quadPos, velocity) * velocity * particle_motion_blur_amount;

  output.Pos = float4(particle_position, 1);
	output.Pos = mul(output.Pos, camera_view);
	output.Pos.xyz += quadPos;
	output.Pos = mul(output.Pos, camera_proj);
  output.worldPos = quadPos + particle_position;

  float3 speed = iColor.rgb;

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

  float3 camera2wpos = input.worldPos - camera_pos;
  float particleDepth = dot(camera_front.xyz, camera2wpos) / camera_zfar;

  float alpha = 1;
  
  if(particleDepth >= zlinear){ //La particula esta detras
    float depthDiff = particleDepth - zlinear;
    float fadeDistance = 0.5;
    float maxDepthDiff = fadeDistance / camera_zfar;
    alpha = lerp(0, 1, 1 - abs(clamp(depthDiff, 0, maxDepthDiff)) / maxDepthDiff);
  }

  float4 oDiffuse = txAlbedo.Sample(samLinear, finalUV);
  float4 finalColor = oDiffuse * particle_color;
  finalColor.a *= alpha;
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
