#pragma once

#include "ui.h"
#include "textureManager.h"

class ImageDraw : public UI
{
public:
	ImageDraw() {} // デフォコン
	// 引コン

	/*
	ImageDraw(Vector3 pos, Vector3 scale, Vector3 rot, const std::wstring filePath, bool isNoUpdate = false)
	{
		// 引数を受け取り値をセット
		SetPosition(pos);
		SetScale(scale);
		SetRotation(rot);
		SetTextureID(TextureManager::LoadTexture(filePath));
		SetNoUpdate(isNoUpdate);
		SetDefaultVertex();
		//SetActive(true);
		// アップデートしないが有効なら初期化処理へ(そこに描画を置く)
		if (isNoUpdate)
		{
			Init();
		}
	}
	*/

	void FirstInit(Vector3 pos, Vector3 scale, Vector3 rot, const std::wstring filePath, bool isNoUpdate = false)
	{
		// 引数を受け取り値をセット
		SetPosition(pos);
		SetScale(scale);
		SetRotation(rot);
		SetTextureID(TextureManager::LoadTexture(filePath));
		SetNoUpdate(isNoUpdate);
		SetDefaultVertex();
		SetActive(true);
		// アップデートしないが有効なら初期化処理へ(そこに描画を置く)
		if (isNoUpdate)
		{
			Init();
		}
	}
	void Init() override; // 画像登録処理
	void Uninit() override; // 画像終了処理
	void Update() override; // 動かすなら、、というか動く系のUIを作る場合は別途overrideかな？
	// moveuiみたいなクラス作ってあげればa->bに動かすとかできるねぇ、でもその場合はunityと同様に独自でクラス(script)作れば良い気が
	void Draw() override; // 画像描画処理

};