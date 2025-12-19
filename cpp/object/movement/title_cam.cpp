#include "main.h"
#include "object/movement/title_cam.h"

void TitleCam::Attach(GameObject* pointer, const Vector3& start_pos)
{
	// pointerを元にアタッチ処理
	m_AttachObject = pointer;

	// リストクリアしてリスト作成
	m_MovementList.clear();
	// 地点追加
	m_MovementList.push_back(std::make_pair(start_pos, 0.0f));
	m_MovementList.push_back(std::make_pair(Vector3(0.0f, 0.0f, -10.0f) + start_pos, 1200.0f));
	m_MovementList.push_back(std::make_pair(Vector3(-10.0f, 0.0f, -10.0f) + start_pos, 1200.0f));
	m_MovementList.push_back(std::make_pair(Vector3(start_pos), 1500.0f));

	// 変数初期化
	m_Count = 0;
	m_CurrentIndex = 0;
}

void TitleCam::Detach()
{
	// アタッチ解除処理
	m_AttachObject = nullptr;
}

void TitleCam::Move(const float& speed)
{
	// 経過時間加算
	m_Count += speed;

	// 現在の移動区間の情報を取得
	const auto& start_point = m_MovementList[m_CurrentIndex];
	const auto& end_point = m_MovementList[m_CurrentIndex + 1];

	const Vector3& start_pos = start_point.first;
	const Vector3& end_pos = end_point.first;
	const float duration = end_point.second;

	// 補間率を計算
	float t = 0.0f;
	if (duration > 0.0f)
	{
		t = m_Count / duration;
	}

	if (t >= 1.0f)
	{
		// 次の区間へ
		t = 1.0f; // ちょうど終点に位置させる
		m_CurrentIndex++;
		m_Count = 0.0f; // 経過時間をリセット
		// 最後の区間を超えたらループさせる
		if (m_CurrentIndex >= static_cast<int>(m_MovementList.size()) - 1)
		{
			m_CurrentIndex = 0;
		}
	}

	// 線形補間で新しい位置を計算し、オブジェクトに設定
	Vector3 lerped_pos;
	lerped_pos.x = std::lerp(start_pos.x, end_pos.x, t);
	lerped_pos.y = std::lerp(start_pos.y, end_pos.y, t);
	lerped_pos.z = std::lerp(start_pos.z, end_pos.z, t);

	m_AttachObject->SetPosition(lerped_pos);

}