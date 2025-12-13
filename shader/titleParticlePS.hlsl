#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // 加算合成しなくてもぽいので加算合成なしで(背景に制限かかっちゃうから)

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
    
    // グラデーションする
    float linearGradient = saturate(1.0 - (In.TexCoord.x + In.TexCoord.y) * 0.5);
    
    // powで補正(グラデーションを鋭く)
    linearGradient = pow(linearGradient, 1.35f);
    
    // 発光強度設定
    float intensity = 3.0f;
    
    // 合成
    outDiffuse.rgb *= outDiffuse.rgb * linearGradient * intensity;
}