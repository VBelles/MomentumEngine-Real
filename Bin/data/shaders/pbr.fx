#include "common.fx"

#define PI 3.14159265359f


//--------------------------------------------------------------------------------------
// GBuffer generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_GBuffer(
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

void VS_GBuffer_Skin(
	in float4 iPos     : POSITION
	, in float3 iNormal : NORMAL0
	, in float2 iTex0 : TEXCOORD0
	, in float2 iTex1 : TEXCOORD1
	, in float4 iTangent : NORMAL1
	, int4   iBones : BONES
	, float4 iWeights : WEIGHTS

	, out float4 oPos : SV_POSITION
	, out float3 oNormal : NORMAL0
	, out float4 oTangent : NORMAL1
	, out float2 oTex0 : TEXCOORD0
	, out float2 oTex1 : TEXCOORD1
	, out float3 oWorldPos : TEXCOORD2
) {
	float4x4 skin_mtx = getSkinMtx(iBones, iWeights);
	float4 world_pos = mul(iPos, skin_mtx);

	oPos = mul(world_pos, camera_view_proj);

	// Rotar la normal segun la transform del objeto, en este caso dictada por la matriz de skinning
	oNormal = mul(iNormal, (float3x3)skin_mtx);
	oTangent.xyz = mul(iTangent.xyz, (float3x3)skin_mtx);
	oTangent.w = iTangent.w;

	// Las uv's se pasan directamente al ps
	oTex0 = iTex0;
	oTex1 = iTex1;
	oWorldPos = world_pos.xyz;
}

// float2 parallaxMapping(in float3 V, in float2 T)
// {
//    // determine optimal number of layers
//    const float minLayers = 10;
//    const float maxLayers = 64;
//    float numLayers = lerp(maxLayers, minLayers, abs(dot(float3(0, 0, 1), V)));

//    // height of each layer
//    float layerHeight = 1.0 / numLayers;
//    // current depth of the layer
//    float curLayerHeight = 0;
//    // shift of texture coordinates for each layer
//    float parallax_scale = 0.0002;
//    float2 dtex = parallax_scale * V.xy / numLayers;
 
//    // current texture coordinates
//    float2 currentTextureCoords = T;

//    // depth from heightmap
//    float heightFromTexture = txHeight.Sample(samLinear, currentTextureCoords).r;

//     // while point is above the surface
//    [unroll(230)] while(heightFromTexture > curLayerHeight)
//    {
//       // to the next layer
//       curLayerHeight += layerHeight; 
//       // shift of texture coordinates
//       currentTextureCoords -= dtex;
//       // new depth from heightmap
//       heightFromTexture = txHeight.Sample(samLinear, currentTextureCoords).r;
//    }
//    ///////////////////////////////////////////////////////////

//    // previous texture coordinates
//    float texStep = dtex / 64.0;
//    float2 prevTCoords = currentTextureCoords + texStep;

//    // heights for linear interpolation
//    float nextH	= heightFromTexture - curLayerHeight;
//    float prevH	= (txHeight.Sample(samLinear, prevTCoords).r)
//                            - curLayerHeight + layerHeight;

//    // proportions for linear interpolation
//    float weight = nextH / (nextH - prevH);

//    // interpolation of texture coordinates
//    float2 finalTexCoords = prevTCoords * weight + currentTextureCoords * (1.0-weight);

//    // return result
//    return finalTexCoords;
// }

//--------------------------------------------------------------------------------------
// GBuffer generation pass. Pixel shader
//--------------------------------------------------------------------------------------
void PS_GBuffer(
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

	float3 camera2wpos = iWorldPos - camera_pos;
	// float3x3 TBN = computeTBN(iNormal, iTangent);
	// float3x3 wTBN = transpose(TBN);
	// float3 view_dir = normalize(mul(camera_pos, wTBN) - mul(iWorldPos, wTBN));

	//iTex0 = parallaxMapping(view_dir, iTex0);

	// Store in the Alpha channel of the albedo texture, the 'metallic' amount of
	// the material
	o_albedo = txAlbedo.Sample(samLinear, iTex0) * obj_color;
	o_albedo.a = txMetallic.Sample(samLinear, iTex0).r;

	float3 N = computeNormalMap(iNormal, iTangent, iTex0);

	// Save roughness in the alpha coord of the N render target
	float roughness = txRoughness.Sample(samLinear, iTex0).r;
	o_normal = encodeNormal(N, roughness);

	o_selfIllum = txSelfIllum.Sample(samLinear, iTex0) * float4(self_illum_tint, 1) * self_illum_ratio;

	// REMOVE ALL THIS
	// Si el material lo pide, sobreescribir los valores de la textura
	// por unos escalares uniformes. Only to playtesting...
	//TODO quitar en la versión final
	if (scalar_metallic >= 0.f)
		o_albedo.a = scalar_metallic;
	if (scalar_roughness >= 0.f)
		o_normal.a = scalar_roughness;

	// Compute the Z in linear space, and normalize it in the range 0...1
	// In the range z=0 to z=zFar of the camera (not zNear)
	o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}


//--------------------------------------------------------------------------------------
void PS_GBufferMix(
	float4 Pos : SV_POSITION
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
	// This is different -----------------------------------------
	// Using second set of texture coords
	float4 weight_texture_boost = txMixBlendWeights.Sample(samLinear, iTex1);

	float heightR = txHeight.Sample(samLinear, iTex0).r;
	float heightG = txHeight1.Sample(samLinear, iTex0).r;
	float heightB = txHeight2.Sample(samLinear, iTex0).r;

	// Use the alpha of the albedo as heights + texture blending extra weights + material ctes extra weights (imgui)
	//Not_Juan: Como tenemos el height map a parte, lo usamos en vez de albedo.a
	float w1, w2, w3;
	computeBlendWeights(heightR + mix_boost_r + weight_texture_boost.r
					, 	heightG + mix_boost_g + weight_texture_boost.g
					, 	heightB + mix_boost_b + weight_texture_boost.b
					, 	w1, w2, w3);

	float4 albedoR = txAlbedo.Sample(samLinear, iTex0);
	float4 albedoG = txAlbedo1.Sample(samLinear, iTex0);
	float4 albedoB = txAlbedo2.Sample(samLinear, iTex0);

	// Use the weight to 'blend' the albedo colors
	float4 albedo = albedoR * w1 + albedoG * w2 + albedoB * w3;
	o_albedo.xyz = albedo.xyz * obj_color;

	// Mix the normal
	float3 normalR = txNormal.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
	float3 normalG = txNormal1.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
	float3 normalB = txNormal2.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
	float3 normal_color = normalR * w1 + normalG * w2 + normalB * w3;
	float3x3 TBN = computeTBN(iNormal, iTangent);

	// Normal map comes in the range 0..1. Recover it in the range -1..1
	float3 wN = mul(normal_color, TBN);
	float3 N = normalize(wN);

	// Missing: Do the same with the metallic & roughness channels
	float metallicR = txMetallic.Sample(samLinear, iTex0).r;
	float metallicG = txMetallic1.Sample(samLinear, iTex0).r;
	float metallicB = txMetallic2.Sample(samLinear, iTex0).r;
	o_albedo.a = metallicR * w1 + metallicG * w2 + metallicB * w3;

	float roughnessR = txRoughness.Sample(samLinear, iTex0).r;
	float roughnessG = txRoughness1.Sample(samLinear, iTex0).r;
	float roughnessB = txRoughness2.Sample(samLinear, iTex0).r;
	float roughness = roughnessR * w1 + roughnessG * w2 + roughnessB * w3;
	o_normal = encodeNormal(N, roughness);

	float4 selfIllumR = txSelfIllum.Sample(samLinear, iTex0);
	float4 selfIllumG = txSelfIllum1.Sample(samLinear, iTex0);
	float4 selfIllumB = txSelfIllum2.Sample(samLinear, iTex0);
	o_selfIllum = selfIllumR * w1 + selfIllumG * w2 + selfIllumB * w3;
	o_selfIllum =  o_selfIllum * float4(self_illum_tint, 1) * self_illum_ratio;
	// Possible plain blending without heights
	//o_albedo.xyz = lerp( albedoB.xyz, albedoG.xyz, weight_texture_boost.y );
	//o_albedo.xyz = lerp( o_albedo.xyz, albedoR.xyz, weight_texture_boost.x );

	//o_albedo.xyz = float3( iTex1.xy, 0 );		// Show the texture coords1

	//o_albedo.xyz = weight_texture_boost.xyz;	// Show the extra weight textures

	// Compute the Z in linear space, and normalize it in the range 0...1
	// In the range z=0 to z=zFar of the camera (not zNear)
	float3 camera2wpos = iWorldPos - camera_pos;
	o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}


//--------------------------------------------------------------------------------------
void decodeGBuffer(
	in float2 iPosition          // Screen coords
	, out float3 wPos
	, out float3 N
	, out float3 real_albedo
	, out float3 real_specular_color
	, out float  roughness
	, out float3 reflected_dir
	, out float3 view_dir
	, out float4 self_illum
) {

	int3 ss_load_coords = uint3(iPosition.xy, 0);

	// Recover world position coords
	float  zlinear = txGBufferLinearDepth.Load(ss_load_coords).x;
	wPos = getWorldCoords(iPosition.xy, zlinear);

	// Recuperar la normal en ese pixel. Sabiendo que se
	// guardó en el rango 0..1 pero las normales se mueven
	// en el rango -1..1
	float4 N_rt = txGBufferNormals.Load(ss_load_coords);
	N = decodeNormal(N_rt.xyz);
	N = normalize(N);

	// Get other inputs from the GBuffer
	float4 albedo = txGBufferAlbedos.Load(ss_load_coords);
	// In the alpha of the albedo, we stored the metallic value
	// and in the alpha of the normal, we stored the roughness
	float  metallic = albedo.a;
	roughness = N_rt.a;

	self_illum = txGBufferSelfIllum.Load(ss_load_coords);

	// Apply gamma correction to albedo to bring it back to linear.
	albedo.rgb = pow(abs(albedo.rgb), 2.2f);

	// Lerp with metallic value to find the good diffuse and specular.
	// If metallic = 0, albedo is the albedo, if metallic = 1, the
	// used albedo is almost black
	real_albedo = albedo.rgb * (1. - metallic);

	// 0.03 default specular value for dielectric.
	real_specular_color = lerp(0.03f, albedo.rgb, metallic);

	// Eye to object
	float3 incident_dir = normalize(wPos - camera_pos.xyz);
	reflected_dir = normalize(reflect(incident_dir, N));
	view_dir = -incident_dir;
}

// -------------------------------------------------
// Gloss = 1 - rough*rough
float3 Specular_F_Roughness(float3 specularColor, float gloss, float3 h, float3 v) {
	// Sclick using roughness to attenuate fresnel.
	return (specularColor + (max(gloss, specularColor) - specularColor) * pow((1 - saturate(dot(v, h))), 5));
}

float NormalDistribution_GGX(float a, float NdH) {
	// Isotropic ggx.
	float a2 = a * a;
	float NdH2 = NdH * NdH;

	float denominator = NdH2 * (a2 - 1.0f) + 1.0f;
	denominator *= denominator;
	denominator *= PI;

	return a2 / denominator;
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL) {
	// Smith schlick-GGX.
	float k = a * 0.5f;
	float GV = NdV / (NdV * (1 - k) + k);
	float GL = NdL / (NdL * (1 - k) + k);

	return GV * GL;
}


float Specular_D(float a, float NdH) {
	return NormalDistribution_GGX(a, NdH);
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV) {
	return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
}

float3 Fresnel_Schlick(float3 specularColor, float3 h, float3 v) {
	return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Specular_F(float3 specularColor, float3 h, float3 v) {
	return Fresnel_Schlick(specularColor, h, v);
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV) {
	return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, v, h)) / (4.0f * NdL * NdV + 0.0001f);
}

//--------------------------------------------------------------------------------------
// Ambient pass, to compute the ambient light of each pixel
float4 PS_ambient(
	in float4 iPosition : SV_Position
	, in float2 iUV : TEXCOORD0
) : SV_Target
{

	// Declare some float3 to store the values from the GBuffer
	float3 wPos, N, albedo, specular_color, reflected_dir, view_dir;
	float4 self_illum;
	float  roughness;
	decodeGBuffer(iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir, self_illum);

	// if roughness = 0 -> I want to use the miplevel 0, the all-detailed image
	// if roughness = 1 -> I will use the most blurred image, the 8-th mipmap, If image was 256x256 => 1x1
	float mipIndex = roughness * roughness * 8.0f;
	float3 env = txEnvironmentMap.SampleLevel(samLinear, reflected_dir, mipIndex).xyz;
	// Convert the color to linear also.
	env = pow(abs(env), 2.2f);

	// The irrandiance, is read using the N direction.
	// Here we are sampling using the cubemap-miplevel 4, and the already blurred txIrradiance texture
	// and mixing it in base to the scalar_irradiance_vs_mipmaps which comes from the ImGui.
	// Remove the interpolation in the final version!!!
	float3 irradiance_mipmaps = txEnvironmentMap.SampleLevel(samLinear, N, 6).xyz;
	float3 irradiance_texture = txIrradianceMap.Sample(samLinear, N).xyz;
	float3 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * (1. - scalar_irradiance_vs_mipmaps);
	//return float4( irradiance, 1 );
	// How much the environment we see
	float3 env_fresnel = Specular_F_Roughness(specular_color, 1. - roughness * roughness, N, view_dir);

	float g_ReflectionIntensity = 1.0;
	float g_AmbientLightIntensity = 1.0;

	float ao = txAO.Sample(samLinear, iUV).x;

	//return float4(roughness, roughness, roughness, 1 );
	//return float4(self_illum, 1);
	float4 final_color = float4(env_fresnel * env * g_ReflectionIntensity +
								albedo.xyz * irradiance * g_AmbientLightIntensity
								, 1.0f);

	float3 light_dir_full = -light_front;//-light_front;  //float3( 0, 1, 0 ); //light_pos.xyz - wPos;
	float  distance_to_light = length(light_dir_full);
	float3 light_dir = light_dir_full / distance_to_light;
	float intensity = dot(normalize(light_dir), N);
	if (intensity > 0.9) {
		final_color = final_color * global_ambient_adjustment * ao;
	}
	else if (intensity > 0.5) {
		final_color = final_color * global_ambient_adjustment * ao * float4(0.9,0.9,0.9,1);
	}
	else {
		final_color = final_color * global_ambient_adjustment * ao * float4(0.8,0.8,0.8,1);
	}
	return final_color;// + float4((self_illum.xyz * self_illum.a),0);
	//return final_color * global_ambient_adjustment * ao;
}

//--------------------------------------------------------------------------------------
// The geometry that approximates the light volume uses this shader
void VS_pass(
	in float4 iPos : POSITION
	, in float3 iNormal : NORMAL0
	, in float2 iTex0 : TEXCOORD0
	, in float2 iTex1 : TEXCOORD0
	, in float4 iTangent : NORMAL1
	, out float4 oPos : SV_POSITION
) {
	float4 world_pos = mul(iPos, obj_world);
	oPos = mul(world_pos, camera_view_proj);
}

// --------------------------------------------------------
float3 Diffuse(float3 pAlbedo) {
	return pAlbedo / PI;
}

// --------------------------------------------------------
float4 shade(
	float4 iPosition
	, bool   use_shadows
) {
	// Decode GBuffer information
	float3 wPos, N, albedo, specular_color, reflected_dir, view_dir;
	float4 self_illum;
	float  roughness;
	decodeGBuffer(iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir, self_illum);

	// Shadow factor entre 0 (totalmente en sombra) y 1 (no ocluido)
	float shadow_factor = use_shadows ? computeShadowFactor(wPos) : 1.;

	// From wPos to Light
	//camera_front.xyz de la luz
	float3 light_dir_full = light_pos.xyz - wPos;
	float  distance_to_light = length(light_dir_full);
	float3 light_dir = light_dir_full / distance_to_light;

	float  NdL = saturate(dot(N, light_dir));
	float  NdV = lerp(dot(N, view_dir), 1, 0.5);//saturate(dot(N, view_dir));
	float3 h = normalize(light_dir + view_dir); // half vector

	float  NdH = saturate(dot(N, h));
	float  VdH = saturate(dot(view_dir, h));
	float  LdV = saturate(dot(light_dir, view_dir));
	float  a = max(0.001f, roughness * roughness);
	float3 cDiff = Diffuse(albedo);
	float3 cSpec = Specular(specular_color, h, view_dir, light_dir, a, NdL, NdV, NdH, VdH, LdV);

	float  att = (1. - smoothstep(0.90, 0.98, distance_to_light / light_radius));
	 //att *= 1 / distance_to_light;
	//return float4(self_illum, 1);
	float3 final_color = light_color.xyz * NdL * (cDiff * (1.0f - cSpec) + cSpec) * att * light_intensity * shadow_factor;// + (self_illum.xyz * self_illum.a);
	return float4(final_color, 1);
}

float4 PS_point_lights(in float4 iPosition : SV_Position) : SV_Target
{
  return shade(iPosition, false);
}

float4 PS_dir_lights(in float4 iPosition : SV_Position) : SV_Target
{
	// Decode GBuffer information
	float3 wPos, N, albedo, specular_color, reflected_dir, view_dir;
	float4 self_illum;
	float  roughness;
	decodeGBuffer(iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir, self_illum);

	// Shadow factor entre 0 (totalmente en sombra) y 1 (no ocluido)
	float shadow_factor = saturate(0.45 + computeShadowFactor(wPos));

	// From wPos to Light
	//camera_front.xyz de la luz
	float3 light_dir_full = -light_front;//-light_front;  //float3( 0, 1, 0 ); //light_pos.xyz - wPos;
	float  distance_to_light = length(light_dir_full);
	float3 light_dir = light_dir_full / distance_to_light;

	float  NdL = saturate(dot(N, light_dir));
	float  NdV = lerp(dot(N, view_dir), 1, 0.5);//saturate(dot(N, view_dir));
	float3 h = normalize(light_dir + view_dir); // half vector

	float  NdH = saturate(dot(N, h));
	float  VdH = saturate(dot(view_dir, h));
	float  LdV = saturate(dot(light_dir, view_dir));
	float  a = max(0.001f, roughness * roughness);
	float3 cDiff = Diffuse(albedo);
	float3 cSpec = Specular(specular_color, h, view_dir, light_dir, a, NdL, NdV, NdH, VdH, LdV);

	float  att = (1. - smoothstep(0.90, 0.98, distance_to_light / light_radius));
	// att *= 1 / distance_to_light;
	//return float4(self_illum, 1);
	float3 final_color = light_color.xyz * NdL * (cDiff * (1.0f - cSpec) + cSpec) * att * light_intensity * shadow_factor;// + (self_illum.xyz * self_illum.a);
	return float4(final_color, 1);
  //return shade(iPosition, true);
}

float4 PS_dir_lights_player(in float4 iPosition : SV_Position) : SV_Target
{
	// Decode GBuffer information
	float3 wPos, N, albedo, specular_color, reflected_dir, view_dir;
	float4 self_illum;
	float roughness;
	decodeGBuffer(iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir, self_illum);

	// Shadow factor entre 0 (totalmente en sombra) y 1 (no ocluido)
	float shadow_factor = computeShadowFactor(wPos);

	// From wPos to Light
	//camera_front.xyz de la luz
	float3 light_dir_full = -light_front;//-light_front;  //float3( 0, 1, 0 ); //light_pos.xyz - wPos;
	float  distance_to_light = length(light_dir_full);
	float3 light_dir = light_dir_full / distance_to_light;

	float  NdL = saturate(dot(N, light_dir));

	if (NdL < 0.01) {
		shadow_factor = 1;
	}

	if (shadow_factor < 1) {
		if (NdL < 0.2) {
			return float4(0, 0, 0, 0.3);
		}
		else {
			return float4(0, 0, 0, 0.8);
		}
	}
	else {
		return float4(0, 0, 0, 0);
	}
}


// ----------------------------------------
void VS_skybox(
	in float4 iPosition  : POSITION
	, in float4 iColor : COLOR0
	, out float4 oPosition : SV_Position
) {
	// Convert the range 0..1 from iPosition to -1..1 to match the homo space
	oPosition = float4(iPosition.x * 2 - 1., 1 - iPosition.y * 2, 1, 1);
}

// --------------------------------------------------------
float4 PS_skybox(in float4 iPosition : SV_Position) : SV_Target
{
	float3 view_dir = mul(float4(iPosition.xy, 1, 1), camera_screen_to_world).xyz;
	float4 skybox_color = txEnvironmentMap.Sample(samLinear, view_dir);
	return float4(skybox_color.xyz,1) * global_ambient_adjustment;
}
