
#include "common.hlsl"

void main(in DEBUG_PSIN In, out float4 outDiffuse : SV_Target)
{
    if(Parameter.x <= 0.1f)
    {
        // “–‚½‚Á‚Ä‚È‚¢‚Ì‚Å‚»‚Ì‚Ü‚Ü•`‰æ

        // ƒfƒoƒbƒO—p‚É—Î‚ð•`‰æ
        outDiffuse.rb = 0.4f;
        outDiffuse.g = 1.0f;
        outDiffuse.a = 1.0f;

    }
    else
    {
        // “–‚½‚Á‚Ä‚¢‚é‚Ì‚ÅÔ‚­•`‰æ
        outDiffuse.r = 1.0f;
        outDiffuse.gb = 0.4f;
        outDiffuse.a = 1.0f;
    }
}