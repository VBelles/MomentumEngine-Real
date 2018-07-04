#include "common.fx"

//--------------------------------------------------------------------------------------
float4 PS_filter(
    in float4 iPosition : SV_Position
  , in float2 iTex0 : TEXCOORD0
) : SV_Target
{
  float4 color;
  float threshold_min;
  float threshold_max;

  float4 selfIllum = txSelfIllum.Sample(samClampLinear, iTex0);

  if(selfIllum.x > 0 || selfIllum.y > 0 || selfIllum.z > 0){
    color = selfIllum;
    threshold_min = bloom_self_illum_threshold_min;
    threshold_max = bloom_self_illum_threshold_max;
  }
  else{
    color = txBloom0.Sample(samClampLinear, iTex0);
    threshold_min = bloom_threshold_min;
    threshold_max = bloom_threshold_max;
  }

  // Or use any other measure to compute amount of light from rgb
  float lum = dot( color.xyz, float3( 0.2126, 0.7152, 0.0722 ) );

  float amount = smoothstep( threshold_min, threshold_max, lum);
  return float4( color.xyz * amount, 1 );
}

//--------------------------------------------------------------------------------------
float4 PS_add(
    in float4 iPosition : SV_Position
  , in float2 iTex0 : TEXCOORD0
) : SV_Target
{
  float4 blurred_whites0 = txBloom0.Sample(samClampLinear, iTex0);
  float4 blurred_whites1 = txBloom1.Sample(samClampLinear, iTex0);
  float4 blurred_whites2 = txBloom2.Sample(samClampLinear, iTex0);
  float4 blurred_whites3 = txBloom3.Sample(samClampLinear, iTex0);
  return 
    blurred_whites0 * bloom_weights.x + 
    blurred_whites1 * bloom_weights.y + 
    blurred_whites2 * bloom_weights.z + 
    blurred_whites3 * bloom_weights.w 
  ;
}
