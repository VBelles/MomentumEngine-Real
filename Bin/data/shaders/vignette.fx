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


static const float radiusX = 0.7;    
static const float radiusY = 0.2; 
static const float intensity = 0.6; 

float4 PS(
	in float4 iPosition : SV_Position
	, in float2 iTex0 : TEXCOORD0
) : SV_Target
{
    float dist = distance(iTex0, float2(0.5, 0.5));    
    float vig = smoothstep(radiusX, radiusY, dist * intensity); 
    float4 src = txAlbedo.Sample(samClampLinear, iTex0.xy);
    return float4(src.xyz * vig, 1.0);
}
