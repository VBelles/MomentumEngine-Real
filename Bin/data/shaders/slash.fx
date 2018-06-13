#include "common.fx"

void VS(
      in float4 iPos    : POSITION
    , in float3 iNormal : NORMAL
    , in float2 iTex0   : TEXCOORD0
    
    , out float4 oPos   : SV_POSITION
    , out float2 oTex0  : TEXCOORD0
){
    oPos = mul(iPos, obj_world);
    oPos = mul(oPos, camera_view);
    oPos = mul(oPos, camera_proj);
    oTex0 = iTex0;
}

float4 PS(
      float4 oPos   : SV_POSITION
    , float2 iTex0  : TEXCOORD0
    ) : SV_Target { 
    float4 oDiffuse = txAlbedo.Sample(samClampLinear, iTex0);
    return oDiffuse;
}