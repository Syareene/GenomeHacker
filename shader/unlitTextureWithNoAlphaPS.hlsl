
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        
        // alpha’l‚ª0.1–¢–ž‚È‚ç•`‰æ‚µ‚È‚¢‚æ‚¤‚É
        if(outDiffuse.a <= 0.1f)
        {
            discard;
        }
        //clip();->‚±‚Á‚¿‚Å‚à
        
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
}
