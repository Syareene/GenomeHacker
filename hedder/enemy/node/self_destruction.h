#pragma once

#include "enemy/node/base.h"

class SelfDestruction : public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	void ShowConfigWindow() override; // ノード固有のパラメータをセットするデバッグ用ウィンドウを出す関数
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetMoveVal(const float val) 
	{ 
		m_MoveVal = val;
		// データを更新したため説明文も更新
		GenerateDescriptionText();
	}
	std::string GenerateDescriptionText() override; // ノードごとの説明文設定関数

private:
	float m_MoveVal = 1.0f; // 球の速度
	float m_ShotInterval = 60.0f; // 発射間隔(フレーム数)
	float m_BulletDamage = 1.0f; // 弾のダメージ(仮)
};