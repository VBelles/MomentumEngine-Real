#ifndef INC_RENDER_CTES_H_ 
#define INC_RENDER_CTES_H_ 

// -------------------------------------------------
// Constant Buffers
#define CB_CAMERA       0
#define CB_OBJECT       1
#define CB_SKIN_BONES   2
#define CB_LIGHT        3
#define CB_GLOBALS      4
#define CB_MATERIAL     5
#define CB_BLUR         6
#define CB_BLOOM        7
#define CB_FOCUS        8
#define CB_GUI          9
#define CB_FOG          10
#define CB_PARTICLE     11
#define CB_SLASH        12

// -------------------------------------------------
// Texture Slots
// Materials
#define TS_ALBEDO				0
#define TS_NORMAL				1
#define TS_METALLIC				2
#define TS_ROUGHNESS			3
#define TS_SELF_ILLUM			4
#define TS_HEIGHT				5
#define TS_DETAIL_NORMAL		6

// .. Other slots of the material
#define TS_NUM_MATERIALS_SLOTS 7

// Here goes the slots NOT associated to a material
#define TS_LIGHT_PROJECTOR     8
#define TS_LIGHT_SHADOW_MAP    9 
#define TS_ENVIRONMENT_MAP     10
#define TS_ENVIRONMENT_MAP_1     11
//#define TS_IRRADIANCE_MAP      11
#define TS_NOISE_MAP           12

#define TS_DEFERRED_ALBEDOS				13
#define TS_DEFERRED_NORMALS				14
#define TS_DEFERRED_LINEAR_DEPTH		15
#define TS_DEFERRED_SELF_ILLUM			16
#define TS_DEFERRED_HEIGHT				17
#define TS_DEFERRED_ACC_LIGHTS			18
#define TS_DEFERRED_AO					19

// LookUpTable for the color grading
#define TS_LUT_COLOR_GRADING          20

#define TS_MIX_BLEND_WEIGHTS          20

#define TS_FIRST_SLOT_MATERIAL_0      TS_ALBEDO
#define TS_FIRST_SLOT_MATERIAL_1      TS_ALBEDO1
#define TS_FIRST_SLOT_MATERIAL_2      TS_ALBEDO2

#define TS_ALBEDO1		21
#define TS_NORMAL1		22
#define TS_METALLIC1	23
#define TS_ROUGHNESS1	24
#define TS_SELF_ILLUM1	25
#define TS_HEIGHT1		26

#define TS_ALBEDO2		27
#define TS_NORMAL2		28
#define TS_METALLIC2	29
#define TS_ROUGHNESS2	30
#define TS_SELF_ILLUM2	31
#define TS_HEIGHT2		32

// -------------------------------------------------
#define RO_COMPLETE     0
#define RO_ALBEDO       1
#define RO_NORMAL       2
#define RO_ROUGHNESS    3
#define RO_METALLIC     4
#define RO_WORLD_POS    5
#define RO_DEPTH_LINEAR 6
#define RO_AO           7

// -------------------------------------------------
#define MAX_SUPPORTED_BONES        128

#ifdef WIN32

#define CB_DEF(xName,idx)   struct xName

#else

// The register ctes in DX11 MUST be called b0, b1, b2, b3....
// b##idx generates the value b##0 -> b0  (## joins to texts)
// CB_CAMERA is defined to be 0 in the top of this document
#define CB_DEF(xName,idx)   cbuffer xName : register(b ## idx)
// cbuffer ConstantBuffer : register(b0)
#define MAT44                     matrix
#define VEC4                      float4
#define VEC3                      float3
#define VEC2                      float2
#define SLOT(slot_number)                 : register(t##slot_number)

#endif

// -------------------------------------------------
CB_DEF(CCteCamera, CB_CAMERA)   // Generates the b0
{
    MAT44 camera_view;
    MAT44 camera_proj;
    MAT44 camera_view_proj;
    MAT44 camera_screen_to_world;
    MAT44 camera_proj_with_offset;      // Used by ao
    VEC3  camera_pos;
    float camera_dummy1;
    VEC3  camera_front;
    float camera_dummy2;
    VEC3  camera_left;
    float camera_dummy3;
    VEC3  camera_up;
    float camera_dummy4;

    float camera_zfar;
    float camera_znear;
    float camera_tan_half_fov;
    float camera_aspect_ratio;

    VEC2  camera_inv_resolution;
    float camera_dummy5;
    float camera_dummy6;
};

CB_DEF(CCteObject, CB_OBJECT)   // Generates the b1
{
    MAT44 obj_world;
    VEC4  obj_color;
	VEC3  self_illum_tint;
	float self_illum_ratio;
};

CB_DEF(CCteSkinBones, CB_SKIN_BONES) {
    MAT44 Bones[MAX_SUPPORTED_BONES];
};

CB_DEF(CCteLight, CB_LIGHT) {
    VEC4  light_color;
    VEC3  light_pos;
    float light_intensity;
    // Align x4
    float light_shadows_step;
    float light_shadows_inverse_resolution;     // 1.0f / 256.0
    float light_shadows_step_with_inv_res;      // light_shadows_step * light_shadows_inverse_resolution
    float light_radius;                         // Used by point lights
    // Align x4
    MAT44 light_view_proj_offset;
    //Posible bool para saber si es ortográfica en vez de dummy?
    VEC3 light_front;
    float dummy;
};

CB_DEF(CCteGlobals, CB_GLOBALS) {
    float global_world_time;
    float global_exposure_adjustment;
    float global_ambient_adjustment;
    float global_hdr_enabled;

    float global_gamma_correction_enabled;
    float global_tone_mapping_mode;
    int   global_render_output;
    float global_skybox_ratio;

    float global_shared_fx_amount;
    float global_shared_fx_val1;
    float global_shared_fx_val2;
    float global_shared_fx_val3;
};

CB_DEF(CCteMaterial, CB_MATERIAL) {
    float  scalar_roughness;
    float  scalar_metallic;
    float  scalar_irradiance_vs_mipmaps;
    float  material_dummy;

    float  mix_boost_r;
    float  mix_boost_g;
    float  mix_boost_b;
    float  material_pad1;
};

CB_DEF(CCteBlur, CB_BLUR) {
    VEC4 blur_w;        // weights
    VEC4 blur_d;        // distances for the 1st, 2nd and 3rd tap
    VEC2 blur_step;     // Extra modifier
    VEC2 blur_center;   // To keep aligned x4
};

CB_DEF(CCteBloom, CB_BLOOM) {
    VEC4  bloom_weights;
    float bloom_threshold_min;
    float bloom_threshold_max;
    float bloom_self_illum_threshold_min;
    float bloom_self_illum_threshold_max;
};

CB_DEF(CCteFocus, CB_FOCUS) {
    float focus_z_center_in_focus;
    float focus_z_margin_in_focus;
    float focus_transition_distance;
    float focus_modifier;
};

CB_DEF(CCteGUI, CB_GUI) {
    VEC2 minUV;
    VEC2 maxUV;
    VEC4 tint_color;
};

CB_DEF(CCteFog, CB_FOG) {
	VEC4 fogColor;
	// Align x4
	float fogDensity;
	float fogMaxRatio;
	float fog_pad1;
	float fog_pad2;
};

CB_DEF(CCteParticle, CB_PARTICLE) {
    VEC2 particle_minUV;
    VEC2 particle_maxUV;
    VEC4 particle_color;
};

CB_DEF(CCteSlash, CB_SLASH) {
	float slash_duration;
	float slash_fadeout_multiplier;
	float slash_pad2;
	float slash_pad3;
};

#endif
