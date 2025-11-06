
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
Texture2D g_EnvTexture : register(t1);
SamplerState	g_SamplerState : register(s0);

static float PI = 3.14159265359f;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // 視線ベクトル
    float3 eyev = normalize(In.WorldPosition.xyz - CameraPosition.xyz);
    
    // 法線ベクトル
    float3 normal = normalize(In.Normal);
    
    // 反射ベクトル
    float3 refv = reflect(eyev.xyz, normal);
    refv = normalize(refv);
    
    // 環境マッピングテクスチャ座標
    float2 envTexCoord;
    envTexCoord.x = atan2(refv.x, refv.z) / (PI * 2);
    envTexCoord.y = acos(refv.y) / PI;

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, envTexCoord);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }

    // プログラム用のフレネル反射式を用いてカメラのviewと法線の角度から反射率を計算してあげると
    // いい感じに反射する
}
