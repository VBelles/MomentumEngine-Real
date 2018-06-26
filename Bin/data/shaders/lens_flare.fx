#include "common.fx"

//https://mynameismjp.wordpress.com/2009/12/15/more-post-processing-tricks-lens-flare/
const static float4 vPurple = float4(0.7f, 0.2f, 0.9f, 1.0f);
const static float4 vOrange = float4(0.7f, 0.4f, 0.2f, 1.0f);
const static float fThreshold = 0.1f;

float4 PS(
    in float4 iPosition : SV_Position
  , in float2 iTex0 : TEXCOORD0
) : SV_Target
{
    /*float2 vMirrorCoord = float2(1.0f, 1.0f) - in_vTexCoord;
    float2 vNormalizedCoord = vMirrorCoord * 2.0f - 1.0f;
    vNormalizedCoord *= fTexScale;
    
    // We'll blur towards the center of screen, and also away from it.
    
    float2 vTowardCenter = normalize(-vNormalizedCoord);
    float2 fBlurDist = fBlurScale * NumSamples;
    float2 vStartPoint = vNormalizedCoord + ((vTowardCenter / g_vSourceDimensions) * fBlurDist);
    float2 vStep = -(vTowardCenter / g_vSourceDimensions) * 2 * fBlurDist;
    
    // Do the blur and sum the samples
    float4 vSum = 0;
    float2 vSamplePos = vStartPoint;
    for (int i = 0; i < NumSamples; i++)
    {
    float2 vSampleTexCoord = vSamplePos * 0.5f + 0.5f;
    
    // Don't add in samples past texture border
    if (vSampleTexCoord.x >= 0 && vSampleTexCoord.x <= 1.0f
    && vSampleTexCoord.y >=0 && vSampleTexCoord.y <= 1.0f)
    {
    float4 vSample = tex2D(PointSampler0, vSampleTexCoord);
    vSum +=  max(0, vSample - fThreshold) * lensFlareColor;
    }
    
    vSamplePos += vStep;
    }
    
    return vSum / NumSamples;*/
    return txAlbedo.Sample(samLinear, iTex0); 
}
