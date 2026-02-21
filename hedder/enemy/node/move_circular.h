#pragma once

#include "enemy/node/base.h"
#include <string>

class FieldEnemy;

class MoveCircular : public NodeBase
{
private:
	float m_Radius = 3.0f;           // 円の半径
	int m_Duration = 180;         // 一周にかかる時間(フレーム)

public:
	void Init(Transform trans) override;
	void ShowConfigWindow() override; // ノード固有のパラメータをセットするデバッグ用ウィンドウを出す関数
	bool NodeEffect(FieldEnemy* enemy_ptr) override;
	std::string GenerateDescriptionText();

	// 移動ベクトルを生成する関数
	Vector3 GenerateMovementVector(FieldEnemy* enemy_ptr);

	// パラメータ設定用
	void SetRadius(float radius) 
	{ 
		m_Radius = radius; 
		// データを更新したため説明文も更新
		GenerateDescriptionText();
	}
	void SetDuration(int duration) 
	{ 
		m_Duration = duration; 
		// データを更新したため説明文も更新
		GenerateDescriptionText();
	}
};