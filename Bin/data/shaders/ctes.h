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

// -------------------------------------------------
// Texture Slots
// Materials
#define TS_ALBEDO              0
#define TS_NORMAL              1
#define TS_METALLIC            2
#define TS_ROUGHNESS           3

// .. Other slots of the material
#define TS_NUM_MATERIALS_SLOTS 4

// Here goes the slots NOT associated to a material
#define TS_LIGHT_PROJECTOR     5
#define TS_LIGHT_SHADOW_MAP    6 
#define TS_ENVIRONMENT_MAP     7
#define TS_IRRADIANCE_MAP      8
#define TS_NOISE_MAP           9

#define TS_DEFERRED_ALBEDOS           10
#define TS_DEFERRED_NORMALS           11
#define TS_DEFERRED_LINEAR_DEPTH      12
#define TS_DEFERRED_ACC_LIGHTS        13

// -------------------------------------------------
#define RO_COMPLETE     0
#define RO_ALBEDO       1
#define RO_NORMAL       2
#define RO_ROUGHNESS    3
#define RO_METALLIC     4
#define RO_WORLD_POS    5
#define RO_DEPTH_LINEAR 6

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
  VEC3  camera_pos;
  float camera_dummy1;
  VEC3  camera_front;
  float camera_dummy2;

  float camera_zfar;
  float camera_znear;
  float camera_tan_half_fov;
  float camera_aspect_ratio;

  VEC2  camera_inv_resolution;
  float camera_dummy3;
  float camera_dummy4;
};

CB_DEF(CCteObject, CB_OBJECT)   // Generates the b1
{
  MAT44 obj_world;
  VEC4  obj_color;
};

CB_DEF(CCteSkinBones, CB_SKIN_BONES)
{
  MAT44 Bones[MAX_SUPPORTED_BONES];
};

CB_DEF(CCteLight, CB_LIGHT)
{
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

CB_DEF(CCteGlobals, CB_GLOBALS)
{
  float global_world_time;
  float global_exposure_adjustment;
  float global_ambient_adjustment;
  float global_hdr_enabled;
  float global_gamma_correction_enabled;
  float global_tone_mapping_mode;
  int   global_render_output;
  float global_dummy2;
};

CB_DEF(CCteMaterial, CB_MATERIAL) 
{
  float  scalar_roughness;
  float  scalar_metallic;
  float  scalar_irradiance_vs_mipmaps;
  float  material_dummy;
};

CB_DEF(CCteBlur, CB_BLUR)
{
  VEC4 blur_w;        // weights
  VEC4 blur_d;        // distances for the 1st, 2nd and 3rd tap
  VEC2 blur_step;     // Extra modifier
  VEC2 blur_center; // To keep aligned x4
};

#endif
