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


static const float radiusX = 0.8;    
static const float radiusY = 0.6; 
static const float intensity = 0.7;

static const float OuterVig = 1.0;
static const float InnerVig = 0.5;

float4 PS(
	in float4 iPosition : SV_Position
	, in float2 iTex0 : TEXCOORD0
) : SV_Target
{
    float4 src = txAlbedo.Sample(samClampLinear, iTex0.xy);

    float dist = distance(iTex0, float2(0.5, 0.5));    
    float vig = smoothstep(radiusX, radiusY, dist * intensity); 
 
    // https://www.shadertoy.com/view/Mdsfzl
		
	//float dist  = distance(iTex0.xy, float2(0.5, 0.5)) * 1.414213; //Multiplyed by 1.414213 to fit in the range of 0.0 to 1.0 
	//float vig = clamp((OuterVig - dist) / (OuterVig - InnerVig), 0.0, 1.0); // Generate the Vignette with Clamp which go from outer Viggnet ring to inner vignette ring with smooth steps
	
    return float4(src.xyz * vig, 1.0);
 
    
}
