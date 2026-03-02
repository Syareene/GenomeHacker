#pragma once
#include "main.h"
#include <functional>


// インスタンシングレンダリングにて用いるgpu受け渡し用バッファの構造体
struct InstanceBufferData
{
	XMFLOAT4X4 WorldMatrix; // 4x4行列
	XMFLOAT4 Color; // rgba
	XMFLOAT4 UVOffset; // xy: offset, zw: scale
	
};

struct RenderQueueData
{
	int Layer; // layerNo
	float Depth; // カメラからの距離
	std::function<void()> DrawCall; // 実際の描画関数

	// 自作構造体なため、ソート用の比較演算子を定義
	bool operator<(const RenderQueueData& other) const
	{
		if (Layer != other.Layer)
		{
			return Layer < other.Layer; // レイヤーで比較
		}
		return Depth < other.Depth; // レイヤーが同じ場合は深度で比較
	}
};