
#include "common.hlsl"

void main(in DEBUG_PSIN In, out float4 outDiffuse : SV_Target)
{
    // デバッグ用に緑を描画
    outDiffuse.rb = 0.4f;
    outDiffuse.g = 1.0f;
    outDiffuse.a = 1.0f;
}
