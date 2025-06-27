#pragma once

#include "main.h"
#include "renderer.h"

class ShaderManager
{
public:
	// 一旦どこでも使unlitをstaticで持っておくだけのヘッダーとする
	// もし使うシェーダーの種類が増えてきたらそのときにflyweightに変更。
	static ID3D11VertexShader* UnlitVertexShader;
	static ID3D11PixelShader* UnlitPixelShader;
	static ID3D11InputLayout* UnlitVertexLayout;

	static void Init();
	static void Uninit();

	// 頂点自体は各自で生成してもらう?
	// ないしは頂点データ自体も画像は4頂点固定だから基底クラスで生成してそれを使い回すように使用しても良いが。
};