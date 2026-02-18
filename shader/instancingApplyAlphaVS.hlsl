#include "common.hlsl"

StructuredBuffer<InstanceData> g_InstanceData : register(t2);

void main(in VS_IN In, out PS_IN Out)
{
    // 自身のデータを取得
    InstanceData data = g_InstanceData[In.InstanceId];
    
    // 行列を用いて座標変換を行う
    float4 localPos = float4(In.Position.xyz, 1.0f);
    float4 worldPos = mul(localPos, data.WorldMatrix);
    float4 viewPos = mul(worldPos, View);
    Out.Position = mul(viewPos, Projection);
    Out.TexCoord = In.TexCoord + data.UVOffset.xy;
	// In.DiffuseのRGB値とインスタンスのRGB値を乗算し、アルファ値はdataから引っ張る
    Out.Diffuse.rgb = In.Diffuse.rgb * data.Color.rgb;
    Out.Diffuse.a = data.Color.a;

}

