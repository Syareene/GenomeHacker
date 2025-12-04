
#include "common.hlsl"

StructuredBuffer<ParticleInstanceData> g_InstanceData : register(t2);

void main(in VS_IN In, out PS_IN Out)
{
    // scale取得
    float scale = g_InstanceData[In.InstanceId].PositionAndSize.z;
    // スケールをかけてあげる
    float4 scaledPosition = In.Position;
    scaledPosition.xy *= scale;
    // スケール後の座標を行列変換
    Out.Position = mul(scaledPosition, World);
	
    Out.Position.xy += g_InstanceData[In.InstanceId].PositionAndSize.xy;
    Out.Position = mul(Out.Position, View);
    Out.Position = mul(Out.Position, Projection);
	
    Out.TexCoord = In.TexCoord + g_InstanceData[In.InstanceId].UVOffset.xy;
	// In.DiffuseのRGB値とインスタンスのRGB値を乗算し、アルファ値は1.0に固定
    Out.Diffuse.rgb = In.Diffuse.rgb * g_InstanceData[In.InstanceId].Color.rgb;
    Out.Diffuse.a = 1.0f;

}

