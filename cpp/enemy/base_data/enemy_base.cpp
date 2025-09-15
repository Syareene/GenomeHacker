// 敵データ基底クラス
// このデータはシーン側かなんかで常に持っといて、ノードの最新適応状態を見れるようにしておく、あとリソースも使い回せるので疑似flyweightとしても使用可能。
// 敵を出す際はこのデータのポインタを渡して出す感じになる。
// ノードに関してはdnaのボタンを押したときにこのクラスからデータを取ってくる形になるかな。

#include "enemy/base_data/enemy_base.h"
#include "manager.h"
#include "object/game_object.h"
#include "texture_manager.h"

#include "enemy/node_tab/tab_base.h"
#include "enemy/node/base.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

void EnemyBase::Register()
{
	// 登録処理

	// そのenemy固有の情報を登録
	m_DnaScreen = std::make_unique<DnaScreenScript>();
	m_DnaScreen->Init(); // DNAスクリーンの初期化

	// テクスチャ生成
	
	m_TextureID = TextureManager::LoadTexture(L"asset\\texture\\");
	// uvテクスチャの場合はuvのデータも変数に設定する。
	
	// タブが作られたので各ノードに対してenemyの初期ノードを登録しておく。
	
	// ポインタに対して初期ノードを追加する。
	//m_DnaScreen->GetAttackTab()->AddNode<NodeBase>(-1);
}

void EnemyBase::Unregister()
{
	// 登録解除処理
	if (m_DnaScreen)
	{
		m_DnaScreen->Uninit(); // DNAスクリーンの終了処理
		m_DnaScreen.reset(); // スクリーンのポインタを開放
	}

	// テクスチャ解放
	TextureManager::UnloadTexture(GetEnemyTextureID());
}

void EnemyBase::ExecuteAttack(FieldEnemy* enemy_ptr)
{
	// 攻撃ノードの処理を実行
	for (auto& node : m_DnaScreen->GetAttackTab()->GetNodes())
	{
		node->NodeEffect(enemy_ptr); // ノードの効果を実行
	}
}

void EnemyBase::ExecuteMove(FieldEnemy* enemy_ptr)
{
	// 移動ノードの処理を実行
	for (auto& node : m_DnaScreen->GetMoveTab()->GetNodes())
	{
		node->NodeEffect(enemy_ptr); // ノードの効果を実行
	}
}

bool EnemyBase::ExecuteDeath(FieldEnemy* enemy_ptr)
{
	// 死亡ノードの処理を実行
	for (auto& node : m_DnaScreen->GetDeathTab()->GetNodes())
	{
		node->NodeEffect(enemy_ptr);
	}
	return true; // 実行終わったらtrueを返す
}

int EnemyBase::SetTextureID(const std::wstring filePath, std::pair<int, int> texTarget, std::pair<int, int> texCount)
{
	// 本来ならuv設定される場合があるのでuv設定されてなければデフォ頂点のptrコピーでいいが
	// そうでない場合は新たに頂点作って格納しておかないと正常にテクスチャが描画されない。

	// テクスチャIDを設定する関数
	m_TextureID = TextureManager::LoadTexture(filePath);
	if (m_TextureID == -1)
	{
		// テクスチャの読み込みに失敗した場合は-1を返す
		return -1;
	}
	// テクスチャのターゲットとカウントを設定(gameobjectでないと設定できないので保存だけする
	m_TextureTarget = texTarget;
	m_TextureCount = texCount;
	
	return m_TextureID; // 成功した場合はテクスチャIDを返す
}

void EnemyBase::ShowToDnaButton(const Vector2& pos, const std::wstring filePath)
{
	// DNAタブへの遷移ボタンを表示
	// ここでボタンを生成して表示する処理を実装
	m_ToDnaButton = Manager::GetCurrentScene()->AddGameObject<Button>(2);

	m_ToDnaButton->Register([this]() {
		// ボタンがクリックされた時の処理
		ShowDnaScreen();
		}, pos, Vector2(300.0f, 100.0f), Vector2(0.0f, 0.0f), filePath);
	m_ToDnaButton->ChangeTexUV(12, 13, 0, 0); // 保存した変数から値を参照するように変更する。
}

void EnemyBase::HideToDnaButton()
{
	// DNAタブへの遷移ボタンを非表示
	if (m_ToDnaButton)
	{
		m_ToDnaButton->Destroy(); // ボタンを破棄
		m_ToDnaButton = nullptr; // ポインタをnullptrに設定
	}
}

void EnemyBase::ShowDnaScreen()
{
	// DNAタブを表示
	if (m_DnaScreen)
	{
		m_DnaScreen->ShowDnaInfo(); // DNA情報を表示する関数を呼び出す
	}
}

void EnemyBase::HideDnaScreen()
{
	// DNAタブを非表示
	if (m_DnaScreen)
	{
		m_DnaScreen->HideDnaInfo(); // DNA情報を非表示にする関数を呼び出す	
	}
}