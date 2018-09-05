#include "common.fx"

/// --------------------
struct VS_FULL_OUTPUT {
  float4 Pos   : SV_POSITION;
  float2 UV    : TEXCOORD0;
};

// ----------------------------------------
VS_FULL_OUTPUT VS_GUI(
  float4 iPos   : POSITION,     // 0..1, 0..1, 0 en la z
  float4 iColor : COLOR0
  )
{
  VS_FULL_OUTPUT output = (VS_FULL_OUTPUT)0;
  float4 world_pos = mul(iPos, obj_world);
  output.Pos = mul(world_pos, camera_view_proj);
  output.UV  = iPos.xy;
  return output;
}

// ----------------------------------------
float4 PS_GUI(
  VS_FULL_OUTPUT input
  ) : SV_Target
{
  float2 finalUV = lerp(minUV, maxUV, input.UV);
  float4 oDiffuse = txAlbedo.Sample(samLinear, finalUV);
  float2 maskUV = lerp(charSize, dummy_GUI, input.UV);
  float4 maskColor = txNormal.Sample(samClampLinear, maskUV);
  float4 oColor = float4(oDiffuse.rgb * tint_color.rgb, oDiffuse.a) * maskColor; 
  return oColor;
}

float4 PS_GUI_FONT(
  VS_FULL_OUTPUT input
  ) : SV_Target
{
  float4 oColor = float4(0,0,0,0);
  if (input.UV.x <= charSize.x && input.UV.y <= charSize.y) {
    float2 finalUV = lerp(minUV, maxUV, input.UV / charSize);
    float4 oDiffuse = txAlbedo.Sample(samLinear, finalUV);
    oColor = float4(oDiffuse.rgb * tint_color.rgb, oDiffuse.a); 
  }
  return oColor;
}