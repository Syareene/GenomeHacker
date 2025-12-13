
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
    // alpha値が一定値以下なら描画しない
    if(outDiffuse.a < 0.1f)
    {
        discard;
    }
    
    
    
    // フォグ(他にもいろいろな計算パターンがあって見え方も異なる)
    //float3 fogColor = float3(1.0f, 1.0f, 1.0f); // 一旦白
    //float fog = In.Depth * 0.02f;
    //outDiffuse.rgb = (outDiffuse.rgb * (1.0 - fog)) + (fogColor * fog);
    //outDiffuse.a = 1.0f;
}
