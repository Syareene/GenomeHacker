
#include "common.hlsl"

void main(in POS_PSIN In, out float4 outDiffuse : SV_Target)
{
    // 特に形は気にせず受け取った値でフェードさせる
    outDiffuse.rgb = 0.0f;
    outDiffuse.a = Parameter.w;
}