// 敵データ基底クラス
// このデータはシーン側かなんかで常に持っといて、ノードの最新適応状態を見れるようにしておく、あとリソースも使い回せるので疑似flyweightとしても使用可能。
// 敵を出す際はこのデータのポインタを渡して出す感じになる。
// ノードに関してはdnaのボタンを押したときにこのクラスからデータを取ってくる形になるかな。

#include "main.h"

#include "enemy/base_data/enemy_base.h"
#include "scene/manager.h"
#include "object/game_object.h"
#include "manager/texture_manager.h"
#include "enemy/dna_screen_script.h"

#include "enemy/node_tab/tab_base.h"
#include "enemy/node/base.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

void EnemyBase::Register()
{
	// 登録処理

	// そのenemy固有の情報を登録
	
	// なんかこのタイミングでオブジェクトとして生成はしといて常駐させつつ、描画とかに必要なデータだけ表示時に受け取って、非表示時に消すとかがいいような
	// 今すぐこのタイミングで作る必要が出てきた、、

	// 常駐させるとするならノードの配置状況とか、のデータとして管理できるものはここでいい
	// 逆にテクスチャとか文字部分の生成に関しては呼ばれた時に行うような処理にしたい
	
	m_DnaScreen = Manager::GetCurrentScene()->AddGameObject<DnaScreenScript>(2); // DNAスクリーンをシーンに追加
	m_DnaScreen->Init(); // DNAスクリーンの初期化

	// テクスチャ生成
	
	//m_TextureID = TextureManager::LoadTexture(L"asset\\texture\\");
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
		m_DnaScreen = nullptr; // スクリーンのポインタを開放
	}

	// テクスチャ解放
	TextureManager::UnloadTexture(GetEnemyTextureID());
}

void EnemyBase::ExecuteAttack(FieldEnemy* enemy_ptr)
{
	// 前のfに保存しているcdmaxと現在のcdmaxが異なるかどうか
	if(enemy_ptr->GetAttackNodeCDSum() != m_DnaScreen->GetAttackTab()->GetCDMax())
	{
		// 時間が伸びる場合はcdmaxだけ更新
		// 短くなる場合は比を使い現在のcd位置も更新
		if (enemy_ptr->GetAttackNodeCDSum() > m_DnaScreen->GetAttackTab()->GetCDMax())
		{
			// 現在のcd位置を更新
			float ratio = static_cast<float>(enemy_ptr->GetAttackNodeCDSum()) / static_cast<float>(m_DnaScreen->GetAttackTab()->GetCDMax());
			enemy_ptr->SetAttackNodeTime(static_cast<int>(enemy_ptr->GetAttackNodeTime() * ratio));

			// 将来的にだけど、変更に伴ってランダムで位置をずらしてもいい説はある
		}

		// 異なる場合は現在のcdmaxを保存
		enemy_ptr->SetAttackNodeCDSum(m_DnaScreen->GetAttackTab()->GetCDMax());
	}

	// 今のcdカウントが最大値を超えているかどうかのチェック
	if (enemy_ptr->GetAttackNodeTime() >= enemy_ptr->GetAttackNodeCDSum())
	{
		// 超えている場合は最大値分引く
		enemy_ptr->SetAttackNodeTime(enemy_ptr->GetAttackNodeTime() - enemy_ptr->GetAttackNodeCDSum());
	}

	int index = 0;
	int beforeTime = 0;
	bool isFinished = false;
	for (auto& time : m_DnaScreen->GetAttackTab()->GetNodeTimeLine())
	{
		if (enemy_ptr->GetAttackNodeTime() <= time)
		{
			// 既に判定が終わっており、前のタイムと現在の時間が同じでない(=次のノードのcdが0でない)場合はループを抜ける
			if (isFinished && beforeTime != time)
			{
				break;
			}
			// シンプルに実行対象or前のノード実行後cdが0のノードを実行
			auto it = m_DnaScreen->GetAttackTab()->GetNodes().begin();
			std::advance(it, index);
			(*it)->NodeEffect(enemy_ptr);

			// ループ抜ける前に色々設定
			beforeTime = time;
			isFinished = true;
		}
		index++;
	}

	// カウントインクリメント
	enemy_ptr->SetAttackNodeTime(enemy_ptr->GetAttackNodeTime() + 1);
}

void EnemyBase::ExecuteMove(FieldEnemy* enemy_ptr)
{
	// 前のfに保存しているcdmaxと現在のcdmaxが異なるかどうか
	if (enemy_ptr->GetMoveNodeCDSum() != m_DnaScreen->GetMoveTab()->GetCDMax())
	{
		// 生成時にfieldEnemyのcdmaxが更新されていないので最初はここはいる


		// 時間が伸びる場合はcdmaxだけ更新
		// 短くなる場合は比を使い現在のcd位置も更新
		if (enemy_ptr->GetMoveNodeCDSum() > m_DnaScreen->GetMoveTab()->GetCDMax())
		{
			// 現在のcd位置を更新
			float ratio = static_cast<float>(enemy_ptr->GetMoveNodeCDSum()) / static_cast<float>(m_DnaScreen->GetMoveTab()->GetCDMax());
			enemy_ptr->SetMoveNodeTime(static_cast<int>(enemy_ptr->GetMoveNodeTime() * ratio));

			// 将来的にだけど、変更に伴ってランダムで位置をずらしてもいい説はある
		}

		// 異なる場合は現在のcdmaxを保存
		enemy_ptr->SetMoveNodeCDSum(m_DnaScreen->GetMoveTab()->GetCDMax());
	}

	// 今のcdカウントが最大値を超えているかどうかのチェック
	if (enemy_ptr->GetMoveNodeTime() >= enemy_ptr->GetMoveNodeCDSum())
	{
		// 超えている場合は最大値分引く
		enemy_ptr->SetMoveNodeTime(enemy_ptr->GetMoveNodeTime() - enemy_ptr->GetMoveNodeCDSum());
	}

	// 移動ノードの処理を実行

	int index = 0;
	int beforeTime = 0;
	bool isFinished = false;
	for (auto& time : m_DnaScreen->GetMoveTab()->GetNodeTimeLine())
	{
		if (enemy_ptr->GetMoveNodeTime() <= time)
		{
			// 既に判定が終わっており、前のタイムと現在の時間が同じでない(=次のノードのcdが0でない)場合はループを抜ける
			if (isFinished && beforeTime != time)
			{
				break;
			}
			// シンプルに実行対象or前のノード実行後cdが0のノードを実行
			m_DnaScreen->GetMoveTab()->GetNodes()[index].get()->NodeEffect(enemy_ptr);

			// ループ抜ける前に色々設定
			beforeTime = time;
			isFinished = true;
		}
		index++;
	}

	// カウントインクリメント
	enemy_ptr->SetMoveNodeTime(enemy_ptr->GetMoveNodeTime() + 1);
}

bool EnemyBase::ExecuteDeath(FieldEnemy* enemy_ptr)
{
	// 前のfに保存しているcdmaxと現在のcdmaxが異なるかどうか
	if (enemy_ptr->GetDeathNodeCDSum() != m_DnaScreen->GetDeathTab()->GetCDMax())
	{
		// 時間が伸びる場合はcdmaxだけ更新
		// 短くなる場合は比を使い現在のcd位置も更新
		if (enemy_ptr->GetDeathNodeCDSum() > m_DnaScreen->GetDeathTab()->GetCDMax())
		{
			// 現在のcd位置を更新
			float ratio = static_cast<float>(enemy_ptr->GetDeathNodeCDSum()) / static_cast<float>(m_DnaScreen->GetDeathTab()->GetCDMax());
			enemy_ptr->SetDeathNodeTime(static_cast<int>(enemy_ptr->GetDeathNodeTime() * ratio));

			// 将来的にだけど、変更に伴ってランダムで位置をずらしてもいい説はある
		}

		// 異なる場合は現在のcdmaxを保存
		enemy_ptr->SetDeathNodeCDSum(m_DnaScreen->GetDeathTab()->GetCDMax());
	}

	// 今のcdカウントが最大値を超えているかどうかのチェック
	if (enemy_ptr->GetDeathNodeTime() >= enemy_ptr->GetDeathNodeCDSum())
	{
		// 超えている場合は最大値分引く
		enemy_ptr->SetDeathNodeTime(enemy_ptr->GetDeathNodeTime() - enemy_ptr->GetDeathNodeCDSum());
	}

	// 死亡ノードの処理を実行
	int index = 0;
	int beforeTime = 0;
	bool isFinished = false;
	for (auto& time : m_DnaScreen->GetDeathTab()->GetNodeTimeLine())
	{
		if (enemy_ptr->GetDeathNodeTime() <= time)
		{
			// 既に判定が終わっており、前のタイムと現在の時間が同じでない(=次のノードのcdが0でない)場合はループを抜ける
			if (isFinished && beforeTime != time)
			{
				break;
			}
			// シンプルに実行対象or前のノード実行後cdが0のノードを実行
			auto it = m_DnaScreen->GetDeathTab()->GetNodes().begin();
			std::advance(it, index);
			(*it)->NodeEffect(enemy_ptr);

			// ループ抜ける前に色々設定
			beforeTime = time;
			isFinished = true;
		}
		index++;
	}

	// カウントインクリメント
	enemy_ptr->SetDeathNodeTime(enemy_ptr->GetDeathNodeTime() + 1);
	return true; // 実行終わったらtrueを返す
}

void EnemyBase::Init()
{
	// 一旦register呼ぶ
	EnemyBase::Register();
}

void EnemyBase::Uninit()
{

}

void EnemyBase::Update()
{
	// DNAタブが表示されている場合は更新処理を行う
	//if (m_IsDnaScreenVisible && m_DnaScreen)
	//{
	//	m_DnaScreen->Update();
	//}
}

void EnemyBase::Draw()
{
	// DNAタブが表示されている場合は描画処理を行う
	//if (m_IsDnaScreenVisible && m_DnaScreen)
	//{
	//	m_DnaScreen->Draw();
	//}
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

void EnemyBase::ShowDnaEditButton(const Vector2& pos, const Vector2& size, const int texID)
{
	// DNAタブへの遷移ボタンを表示
	// ここでボタンを生成して表示する処理を実装
	m_ToDnaButton = Manager::GetCurrentScene()->AddGameObject<Button>(2);

	m_ToDnaButton->Register([this]() {
		// ボタンがクリックされた時の処理
		ShowDnaScreen();
		}, pos, size, Vector2(0.0f, 0.0f), texID, L"asset/texture/test_frame.png");


	// uv変えるのはいいけど、頂点変更モードになってるかだけが疑問やね
	//m_ToDnaButton->ChangeTexUV(12, 13, 0, 0); // 保存した変数から値を参照するように変更する。
	m_ToDnaButton->AddTag("dna");
}

void EnemyBase::HideDnaEditButton()
{
	// DNAタブへの遷移ボタンを非表示
	if (m_ToDnaButton)
	{
		m_ToDnaButton->SetDestory(true); // ボタンを破棄
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