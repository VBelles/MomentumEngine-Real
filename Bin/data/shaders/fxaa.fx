#include "common.fx"


// This shader is expected to be used only with the mesh unitQuadXY.mesh
// Where the iPos goes from 0, 0..1, 1
void VS(
	in float4 iPos : POSITION
	, out float4 oPos : SV_POSITION
	, out float2 oTex0 : TEXCOORD0
) {
	// Passthrough of coords and UV's
	oPos = float4(iPos.x * 2 - 1., 1 - iPos.y * 2, 0, 1);
	oTex0 = iPos.xy;
}

// FXAA basado en https://www.shadertoy.com/view/MdyyRt

static const float2 strength = float2(10.0, 10.0);
static const float reducemul = 0.125; 		// 1.0 / 8.0;
static const float reducemin = 0.0078125;	// 1.0 / 128.0;

float getLum(float3 col){
    return dot(col, float3(0.299, 0.587, 0.114));
}

float4 PS(
	in float4 iPosition : SV_Position
	, in float2 iTex0 : TEXCOORD0
) : SV_Target
{
    float4 Or = txAlbedo.Sample(samClampLinear, iTex0.xy);
    float4 LD = txAlbedo.Sample(samClampLinear, iTex0.xy - camera_inv_resolution);
    float4 RD = txAlbedo.Sample(samClampLinear, iTex0.xy + float2(camera_inv_resolution.x, -camera_inv_resolution.y));
    float4 LT = txAlbedo.Sample(samClampLinear, iTex0.xy + float2(-camera_inv_resolution.x, camera_inv_resolution.y));
    float4 RT = txAlbedo.Sample(samClampLinear, iTex0.xy + camera_inv_resolution);

    float Or_Lum = getLum(Or.xyz);
    float LD_Lum = getLum(LD.xyz);
    float RD_Lum = getLum(RD.xyz);
    float LT_Lum = getLum(LT.xyz);
    float RT_Lum = getLum(RT.xyz);

    float min_Lum = min(Or_Lum, min(min(LD_Lum, RD_Lum), min(LT_Lum, RT_Lum)));
    float max_Lum = max(Or_Lum, max(max(LD_Lum, RD_Lum), max(LT_Lum, RT_Lum)));

    //x direction, - y direction
    float2 dir = float2((LT_Lum + RT_Lum) - (LD_Lum + RD_Lum), (LD_Lum + LT_Lum) - (RD_Lum + RT_Lum));
    float dir_reduce = max((LD_Lum + RD_Lum + LT_Lum + RT_Lum) * reducemul * 0.25, reducemin);
    float dir_min = 1.0 / (min(abs(dir.x), abs(dir.y)) + dir_reduce);
    dir = min(strength, max(-strength, dir * dir_min)) * camera_inv_resolution;

    float4 resultA = 0.5 * (txAlbedo.Sample(samClampLinear, iTex0.xy - 0.166667 * dir) + txAlbedo.Sample(samClampLinear, iTex0.xy + 0.166667 * dir));
    float4 resultB = resultA * 0.5 + 0.25 * (txAlbedo.Sample(samClampLinear, iTex0.xy - 0.5 * dir) + txAlbedo.Sample(samClampLinear, iTex0.xy + 0.5 * dir));
    float B_Lum = getLum(resultB.xyz);

    if(B_Lum < min_Lum || B_Lum > max_Lum)
        return resultA;
    return resultB;
}
