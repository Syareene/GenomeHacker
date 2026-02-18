#include "main.h"
#include "enemy/node/area.h"
#include "enemy/field_enemy.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
#include "collider/collision.h"
#include "enemy/base_data/enemy_base.h"
#include "lib/random_number.h"

// エリアクラス
#include "object/area_object.h"

#include <format>

void Area::Init(Transform trans)
{
	// ベースデータセット

	// 名前
	SetNameData({ "Area", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	// 説明文
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init();
	AddInputTypeTop(InputType::Attack);
	AddInputTypeTop(InputType::Death);
	AddInputTypeBottom(InputType::Attack);
	AddInputTypeBottom(InputType::Death);

	// CDMaxを発射間隔に設定
	SetCDMax(static_cast<int>(m_ShotInterval));
	SetCD(0);
}

bool Area::NodeEffect(FieldEnemy* enemy_ptr)
{
	// 発射処理
	// ダメージエリアを出す
	AreaObject* obj = Manager::GetCurrentScene()->AddGameObject<AreaObject>(1);
	obj->SetOwnerEnemyID(enemy_ptr->GetEnemyBase()->GetEnemyID()); // 発射元の敵種類ID
	// 各種プロパティを設定
	// これ初期化後に呼ばれるからポリゴンのいちおかしくなるかもね
	Vector3 enemy_pos = enemy_ptr->GetPosition();
	Vector3 pos = enemy_pos;
	// x,zはちょいランダム
	pos.x += RandomNumber::GetInstance()->GetRandomFloat(-2.0f, 2.0f);
	pos.z += RandomNumber::GetInstance()->GetRandomFloat(-2.0f, 2.0f);

	pos.y = 0.01f; // 少しだけ地面から浮かせる
	obj->SetPosition(pos);
	obj->SetMaxDuration(static_cast<int>(m_Duration));
	obj->SetAreaDamage(m_AreaDamage);

	return true;
}

std::string Area::GenerateDescriptionText()
{
	// 説明文のテンプレートを取得
	std::string format_string = "このノードがある敵は{}フレーム毎に、{}ダメージを与え{}フレーム持続するエリアを展開します。";
	// std::formatを使用して最終的な文字列を生成
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_ShotInterval, m_AreaDamage, m_Duration));
	// メンバに格納
	SetDescriptionData({ formatted_text, Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	return formatted_text;
}