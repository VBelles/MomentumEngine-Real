//--------------------------------------------------------------------------------------
#include "ctes.h"

//--------------------------------------------------------------------------------------
// from the object material
Texture2D    txAlbedo         	SLOT(TS_ALBEDO);
Texture2D    txNormal         	SLOT(TS_NORMAL);
Texture2D    txMetallic       	SLOT(TS_METALLIC);
Texture2D    txRoughness      	SLOT(TS_ROUGHNESS);
Texture2D    txSelfIllum      	SLOT( TS_SELF_ILLUM );
Texture2D    txHeight      		SLOT( TS_HEIGHT );

// from the light and env
Texture2D    txLightProjector SLOT(TS_LIGHT_PROJECTOR);
Texture2D    txLightShadowMap SLOT(TS_LIGHT_SHADOW_MAP);
TextureCube  txEnvironmentMap SLOT(TS_ENVIRONMENT_MAP);
TextureCube  txIrradianceMap  SLOT(TS_IRRADIANCE_MAP);
Texture2D    txNoiseMap       SLOT(TS_NOISE_MAP);
Texture3D    txLUT            SLOT(TS_LUT_COLOR_GRADING);

// output from deferred
Texture2D    txGBufferAlbedos     	SLOT(TS_DEFERRED_ALBEDOS);
Texture2D    txGBufferNormals     	SLOT(TS_DEFERRED_NORMALS);
Texture2D    txGBufferLinearDepth 	SLOT(TS_DEFERRED_LINEAR_DEPTH);
Texture2D    txGBufferSelfIllum   	SLOT( TS_DEFERRED_SELF_ILLUM );
Texture2D    txGBufferHeight   		SLOT( TS_DEFERRED_HEIGHT );
Texture2D    txAccLights          	SLOT(TS_DEFERRED_ACC_LIGHTS);
Texture2D    txAO                 	SLOT(TS_DEFERRED_AO);

// 2nd material
Texture2D    txAlbedo1         SLOT(TS_ALBEDO1);
Texture2D    txNormal1         SLOT(TS_NORMAL1);
//Texture2D    txMetallic1       SLOT( (TS_FIRST_SLOT_MATERIAL_1 + TS_METALLIC) );
//Texture2D    txRoughness1      SLOT( (TS_FIRST_SLOT_MATERIAL_1 + TS_ROUGHNESS) );

// 3rd material
Texture2D    txAlbedo2         SLOT(TS_ALBEDO2);
Texture2D    txNormal2         SLOT(TS_NORMAL2);
//Texture2D    txMetallic2       SLOT( (TS_FIRST_SLOT_MATERIAL_2 + TS_METALLIC) );
//Texture2D    txRoughness2      SLOT( (TS_FIRST_SLOT_MATERIAL_2 + TS_ROUGHNESS) );

Texture2D    txMixBlendWeights SLOT(TS_MIX_BLEND_WEIGHTS);

// Bloom
Texture2D    txBloom0         SLOT(0);
Texture2D    txBloom1         SLOT(1);
Texture2D    txBloom2         SLOT(2);
Texture2D    txBloom3         SLOT(3);

//--------------------------------------------------------------------------------------
SamplerState samLinear                    : register(s0);
SamplerState samBorderLinear              : register(s1);
SamplerComparisonState samPCF             : register(s2);
SamplerComparisonState samPCFWhiteBorder  : register(s3);
SamplerState samClampLinear               : register(s4);
SamplerState samClampPoint               : register(s5);
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
// Should match the vertex_declaration.cpp @ createNew("Instance", ...
struct TInstanceWorldData {
	float4 InstanceWorld0 : TEXCOORD2;     // Stream 1
	float4 InstanceWorld1 : TEXCOORD3;    // Stream 1
	float4 InstanceWorld2 : TEXCOORD4;    // Stream 1
	float4 InstanceWorld3 : TEXCOORD5;    // Stream 1
};

// Build a World matrix from the instance information
float4x4 getWorldOfInstance(TInstanceWorldData d) {
	return float4x4(d.InstanceWorld0, d.InstanceWorld1, d.InstanceWorld2, d.InstanceWorld3);
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
float4x4 getSkinMtx(int4 iBones, float4 iWeights) {
	// This matrix will be reused for the position, Normal, Tangent, etc
	return  Bones[iBones.x] * iWeights.x
		+ Bones[iBones.y] * iWeights.y
		+ Bones[iBones.z] * iWeights.z
		+ Bones[iBones.w] * iWeights.w;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
float2 hash2(float n) { return frac(sin(float2(n, n + 1.0))*float2(43758.5453123, 22578.1459123)); }

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
float shadowsTap(float2 homo_coord, float coord_z) {
	return txLightShadowMap.SampleCmp(samPCFWhiteBorder, homo_coord, coord_z, 0).x;
}

//--------------------------------------------------------------------------------------
float computeShadowFactor(float3 wPos) {

	// Convert pixel position in world space to light space
	float4 pos_in_light_proj_space = mul(float4(wPos, 1), light_view_proj_offset);
	float3 homo_space = pos_in_light_proj_space.xyz / pos_in_light_proj_space.w; // -1..1

	// Avoid the white band in the back side of the light
	// Nosotros recuperamos el back side porque queremos que haya luz incluso fuera del frustum
	if (pos_in_light_proj_space.z < 0.)
		return 1.f;

	// Poisson distribution random points around a circle
	const float2 offsets[] = {
	  float2(0,0),
	  float2(-0.3700152, 0.575369),
	  float2(0.5462944, 0.5835142),
	  float2(-0.4171277, -0.2965972),
	  float2(-0.8671125, 0.4483297),
	  float2(0.183309, 0.1595028),
	  float2(0.6757001, -0.4031624),
	  float2(0.8230421, 0.1482845),
	  float2(0.1492012, 0.9389217),
	  float2(-0.2219742, -0.7762423),
	  float2(-0.9708459, -0.1171268),
	  float2(0.2790326, -0.8920202)
	};

	// 1./ resolution * custom scale factor
	float coords_scale = light_shadows_step_with_inv_res;

	// Find a random rotation angle based on the world coords
	float angle = 2 * 3.14159268f * hash2(wPos.x + wPos.y).x;
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);

	// Accumulate shadow taps
	float shadow_factor = 0;
	[unroll]
	for (int i = 0; i < 12; ++i) {

		// Get the random offset
		float2 coords = offsets[i];

		// Rotate the offset
		float2 rotated_coord = float2(
			coords.x * cos_angle - coords.y * sin_angle,
			coords.y * cos_angle + coords.x * sin_angle
			);
		// Scane and Translate
		float  tap_sample = shadowsTap(homo_space.xy + rotated_coord * coords_scale, homo_space.z);

		shadow_factor += tap_sample;
	}

	// Divide by the number of taps
	return shadow_factor / 12.f;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
float3x3 computeTBN(float3 inputN, float4 inputT) {
	// Prepare a 3x3 matrix to convert from tangent space to world space
	float3 N = inputN;
	float3 T = inputT.xyz;
	float3 B = cross(N, T) * inputT.w;
	return float3x3(T, B, N);
}

float3 computeNormalMap(float3 inputN, float4 inputT, float2 inUV) {

	float3x3 TBN = computeTBN(inputN, inputT);

	// Normal map comes in the range 0..1. Recover it in the range -1..1
	float3 normal_color = txNormal.Sample(samLinear, inUV).xyz * 2.0 - 1.0;
	float3 wN = mul(normal_color, TBN);
	wN = normalize(wN);

	// Uncomment to disable normal map
	//wN = N;

	return wN;
}


//--------------------------------------------------------------------------------------
// screen_coords va entre 0..1024
//--------------------------------------------------------------------------------------
float3 getWorldCoords(float2 screen_coords, float zlinear_normalized) {

	/*
	  // ux = -1 .. 1
	  // Si screen_coords == 0 => ux = 1
	  // Si screen_coords == 512 => ux = 0
	  // Si screen_coords == 1024 => ux = -1
	  float ux = 1.0 - screen_coords.x * camera_inv_resolution.x * 2;

	  // Si screen_coords =   0 => uy = 1;
	  // Si screen_coords = 400 => uy = 0;
	  // Si screen_coords = 800 => uy = -1;
	  float uy = 1.0 - screen_coords.y * camera_inv_resolution.y * 2;


	  float3 view_dir2 = float3( ux * camera_tan_half_fov * camera_aspect_ratio
							  , uy * camera_tan_half_fov
							  , 1.) * ( zlinear_normalized * camera_zfar );

	  float3 view_dir = mul( float4( screen_coords, 1, 1 ), camera_screen_to_world ).xyz;

	  view_dir *= ( zlinear_normalized );

	  float3 wPos =
		  CameraFront.xyz * view_dir.z
		+ CameraLeft.xyz  * view_dir.x
		+ CameraUp.xyz    * view_dir.y
		+ CameraWorldPos.xyz;
	  return wPos;

	  // camera_screen_to_world includes all the previous operations
	*/

	float3 view_dir = mul(float4(screen_coords, 1, 1), camera_screen_to_world).xyz;
	return view_dir * zlinear_normalized + camera_pos;
}

// -----------------------------------------------------
// Converts range -1..1 to 0..1
float4 encodeNormal(float3 n, float nw) {
	return float4((n + 1.) * 0.5, nw);
}

// Converts range 0..1 to -1..1
float3 decodeNormal(float3 n) {
	return (n.xyz * 2. - 1.);
}

//--------------------------------------------------------------------------------------
void computeBlendWeights( float t1_a
                        , float t2_a
                        , float t3_a
                        , out float w1
                        , out float w2 
                        , out float w3 
                        ) {
  float depth = 0.05;
  float ma = max( t1_a, max( t2_a, t3_a ) ) - depth;
  float b1 = max( t1_a - ma, 0 );
  float b2 = max( t2_a - ma, 0 );
  float b3 = max( t3_a - ma, 0 );
  float b_total = b1 + b2 + b3;
  w1 = b1 / ( b_total );
  w2 = b2 / ( b_total );
  w3 = b3 / ( b_total );
}