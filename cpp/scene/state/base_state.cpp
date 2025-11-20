#include "main.h"
#include "scene/state/base_state.h"

State::~State() = default;

void State::Init()
{
	// override後、個別で追加したいものの追加


	//Scene::Init();
	//Scene::UpdateObjectByTag("system");
}

void State::Uninit()
{
	// override後、個別で追加したいものの追加
}

void State::Update()
{
	// override後、個別で追加したいものの追加

	// また、個別で定義してあるどのupdateを呼ぶかを指定

	// んー、これの欠点としてはscene->stateやstate->sceneの実行順序になってしまうのでレイヤーをちゃんと参照できなさそうなことか
	// まあstate変わっても常駐しているオブジェクトで常に画面の上に描画したいのってuiくらいだから一旦考えなくてもいいかも
}

void State::Draw()
{
	// updateと同じ
}

void State::UpdateFinal()
{
	// systemオブジェクトを更新
	for (auto& systemObject : m_SystemObjects)
	{
		systemObject->UpdateFinal();
	}
}

void State::DeleteGameObject()
{
	// 不要になった3dオブジェクトを削除
	for (auto& objects3d : m_Objects3D)
	{
		// 不要になった GameObject を削除
		objects3d.remove_if([](std::unique_ptr<Object3D>& obj)
			{
				if (obj && obj->Destroy())
				{
					return true;
				}
				return false;
			});
	}

	// 不要になった2dオブジェクトを削除
	for (auto& objects2d : m_Objects2D)
	{
		// 不要になった GameObject を削除
		objects2d.remove_if([](std::unique_ptr<Object2D>& obj)
			{
				if (obj && obj->Destroy())
				{
					return true;
				}
				return false;
			});
	}
}

void State::UpdateStateObject()
{
	// システムオブジェクトの更新
	for (auto& systemObject : m_SystemObjects)
	{
		systemObject->Update();
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			gameObject->Update();
		}
	}

	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			gameObject->Update();
		}
	}

	DeleteGameObject();
}

void State::UpdateStateObjectByTag(const std::string& tag)
{
	// システムオブジェクトの更新
	for (auto& systemObject : m_SystemObjects)
	{
		// タグチェック
		if (!systemObject.get()->IsTagAvailable("system"))
		{
			continue; // 該当タグが見つからなかった場合はスルー
		}
		systemObject->Update();
	}
	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			if (gameObject.get()->IsTagAvailable(tag))
			{
				// 見つかったら実行
				gameObject->Update();
			}
		}
	}
	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			if (gameObject.get()->IsTagAvailable(tag))
			{
				// 見つかったら実行
				gameObject->Update();
			}
		}
	}
	DeleteGameObject();
}

void State::UpdateStateObjectByTags(const std::list<std::string>& tags)
{
	// システムオブジェクトの更新
	for (auto& systemObject : m_SystemObjects)
	{
		// タグチェック
		if (!systemObject.get()->IsTagAvailable("system"))
		{
			continue; // 該当タグが見つからなかった場合はスルー
		}
		systemObject->Update();
	}
	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			for (const auto& tag : tags)
			{
				if (gameObject.get()->IsTagAvailable(tag))
				{
					// 見つかったら実行しこのオブジェクトに対してのこれ以上の探索はしない
					gameObject->Update();
					break;
				}
			}
		}
	}
	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			for (const auto& tag : tags)
			{
				if (gameObject.get()->IsTagAvailable(tag))
				{
					// 見つかったら実行しこのオブジェクトに対してのこれ以上の探索はしない
					gameObject->Update();
					break;
				}
			}
		}
	}

	DeleteGameObject();
}

void State::DrawStateObject()
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			gameObject->Draw();
		}
	}

	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			gameObject->Draw();
		}
	}
}

void State::DrawStateObjectByTag(const std::string& tag)
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			if (gameObject.get()->IsTagAvailable(tag))
			{
				// 見つかったら実行
				gameObject->Draw();
			}
		}
	}
	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			if (gameObject.get()->IsTagAvailable(tag))
			{
				// 見つかったら実行
				gameObject->Draw();
			}
		}
	}
}

void State::DrawStateObjectByTags(const std::list<std::string>& tags)
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			for (const auto& tag : tags)
			{
				if (gameObject.get()->IsTagAvailable(tag))
				{
					// 見つかったら実行しこのオブジェクトに対してのこれ以上の探索はしない
					gameObject->Draw();
					break;
				}
			}
		}
	}
	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			for (const auto& tag : tags)
			{
				if (gameObject.get()->IsTagAvailable(tag))
				{
					// 見つかったら実行しこのオブジェクトに対してのこれ以上の探索はしない
					gameObject->Draw();
					break;
				}
			}
		}
	}
}
