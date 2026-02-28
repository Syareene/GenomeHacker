#include "main.h"
#include "enemy/node/move_random.h"
#include "enemy/field_enemy.h"
#include "scene/manager.h"
#include "lib/random_number.h"
#include "player.h"
#include "imgui.h"

#include <format>

// TODO: 思ってたんと違うのも含めて色々調節する必要あり

void MoveRandom::Init(Transform trans)
{
	// ベースデータセット
	m_MoveVal = DEFAULT_MOVE_VAL; // 移動量

	// 名前
	SetNameData({ "MoveRandom", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal});
	// 説明文
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init();
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
}

void MoveRandom::ShowConfigWindow()
{
    // NodeでのWindow設定適応
    ImWindowSettings();
    // ウィンドウ生成
	ImGui::Begin("MoveRandom Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SeparatorText("Properties");
    // 設定可能なパラメーターを列挙
    if (ImGui::SliderFloat("Move Amount", &m_MoveVal, 0.01f, 0.3f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
    {
        // データを更新したため説明文も更新
        GenerateDescriptionText();
    }
    // どのタブで使えるかを表示
    ShowTabInfo();

	ImGui::End();
}

bool MoveRandom::NodeEffect(FieldEnemy* enemy_ptr)
{
	// moveノードなのでcdはチェックせず常に動かす
	// 通常のノードならcdチェックし動かす形に
	// んで実行したらtrueを返す


    // パーリンノイズが-1~1のときのプログラムなので
    // 今は0-1のため色々と留意

    // 時間取得
    float time = static_cast<float>(enemy_ptr->GetLiveTime());

    // 敵個体のオフセット(インスタンスごとのランダムな値)
    float enemySeed = static_cast<float>(enemy_ptr->GetObjectID() * 100);

    // ノード固有のオフセット
    float nodeOffset = static_cast<float>(GetNodeID() * 10.5f); // 適当に散らす

    // ノイズ取得
    // 座標入力値を大きくずらすことで異なるパターンの場所を参照できる!
    float frequency = 0.05f; // 変化の速度

    // X軸用のノイズ
    double noiseX = RandomNumber::GetInstance()->GetPerlinNoise(
        (time * frequency) + enemySeed + nodeOffset,
        0.0 // Yは固定または別の変動値
    );

    // Z軸用のノイズ(Xとは入力値を十分離す)
    double noiseZ = RandomNumber::GetInstance()->GetPerlinNoise(
        (time * frequency) + enemySeed + nodeOffset,
        100.0 // Yをずらすだけで全く違う波形になる
    );

    // 移動の反映
    Vector3 pos;

    float speed = m_MoveVal;

    pos.x = static_cast<float>(noiseX) * speed;
    pos.z = static_cast<float>(noiseZ) * speed;

    enemy_ptr->AddPosition(pos);

	return true;
}

std::string MoveRandom::GenerateDescriptionText()
{
	// 説明文のテンプレートを取得
	std::string format_string = "このノードがある敵は毎フレーム{:.2f}だけランダムな方向に移動します。";
	// std::formatを使用して最終的な文字列を生成
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_MoveVal));
    // メンバに格納
    SetDescriptionData({ formatted_text, Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	return formatted_text;
}