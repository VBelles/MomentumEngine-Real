#include "common.fx"

void VS(
      in float4 iPos        : POSITION
    , in float3 iNormal     : NORMAL
    , in float2 iTex0       : TEXCOORD0
    
    , out float4 oPos       : SV_POSITION
    , out float3 oNormal    : NORMAL
    , out float2 oTex0      : TEXCOORD0
){
    oPos = mul(iPos, obj_world);
    oPos = mul(oPos, camera_view);
    oPos = mul(oPos, camera_proj);
    oNormal = iNormal;
    oTex0 = iTex0;
}

float4 PS(
      float4 iPos       : SV_POSITION
    , float3 iNormal    : NORMAL
    , float2 iTex0      : TEXCOORD0
    ) : SV_Target {

    float time = iNormal.x;
    float4 textureColor = txAlbedo.Sample(samClampLinear, iTex0);
    float4 color =  textureColor * obj_color;
    color.a -= time / slash_duration;
    return color;
}