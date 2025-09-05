
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // デバッグ用に緑を描画
    outDiffuse.rb = 0.4f;
    outDiffuse.g = 1.0f;
    outDiffuse.a = 1.0f;
}
