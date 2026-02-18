#include "main.h"
#include "enemy/node/self_destruction.h"
#include "enemy/field_enemy.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
#include "collider/collision.h"
#include "enemy/base_data/enemy_base.h"

// 敵専用の弾クラス
#include "enemy/enemy_bullet.h"

#include <format>

void SelfDestruction::Init(Transform trans)
{
	// ベースデータセット

	// 名前
	SetNameData({ "SelfDestruction", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	// 説明文
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init();
	AddInputTypeTop(InputType::Attack);
	AddInputTypeTop(InputType::Death);
	AddInputTypeBottom(InputType::Attack);
	AddInputTypeBottom(InputType::Death);

	m_MoveVal = 0.05f; // 球速度
	m_ShotInterval = 150.0f;

	// CDMaxを発射間隔に設定
	SetCDMax(static_cast<int>(m_ShotInterval));
	SetCD(0);
}

bool SelfDestruction::NodeEffect(FieldEnemy* enemy_ptr)
{
	// 発射処理
	// 8方向に球を出す
	for (int i = 0; i < 8; ++i)
	{
		// 敵専用の弾の生成処理
		EnemyBullet* bullet = Manager::GetCurrentScene()->AddGameObject<EnemyBullet>(1);
		// 弾の初期位置を敵の位置にセット
		Transform trans;
		trans.SetPosition(enemy_ptr->GetPosition());
		trans.SetScale(bullet->GetScale());
		trans.SetRotation(bullet->GetRotation());

		bullet->SetTransform(trans);

		// 弾の発射元の敵種類IDを設定
		if (enemy_ptr->GetEnemyBase())
		{
			bullet->SetOwnerEnemyID(enemy_ptr->GetEnemyBase()->GetEnemyID());
		}

		// 角度に応じてvelocityをセット
		float angle = i * (3.14159f / 4.0f); // 45度刻み
		Vector3 velocity;
		velocity.x = cosf(angle) * m_MoveVal;
		velocity.y = 0.0f;
		velocity.z = sinf(angle) * m_MoveVal;
		// 弾の速度を設定
		bullet->SetVelocity(velocity);
	}

	return true;
}

std::string SelfDestruction::GenerateDescriptionText()
{
	// 説明文のテンプレートを取得
	std::string format_string = "このノードがある敵は{}フレーム毎に8方向に進む球を出します。";
	// std::formatを使用して最終的な文字列を生成
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_ShotInterval));
	// メンバに格納
	SetDescriptionData({ formatted_text, Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	return formatted_text;
}