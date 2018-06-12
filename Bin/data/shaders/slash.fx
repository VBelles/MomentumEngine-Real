#include "common.fx"

struct VS_OUTPUT{
    float4 Pos : SV_POSITION;
    float2 Tex0 : TEXCOORD0;
};


VS_OUTPUT VS(
  in float4 iPos     : POSITION
, in float3 iNormal  : NORMAL0
, in float2 iTex0    : TEXCOORD0
){
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(iPos, obj_world);
    output.Pos = mul(output.Pos, camera_view);
    output.Pos = mul(output.Pos, camera_proj);
    output.Tex0 = iTex0;
    return output;
}

float4 PS(
    VS_OUTPUT input
    ) : SV_Target { 
    //float4 oDiffuse = txAlbedo.Sample(samClampLinear, input.Tex0);
    //return oDiffuse; 
    return float4(1,1,1,1);
}