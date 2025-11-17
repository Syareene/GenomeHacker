#pragma once

#include <list>
#include <memory>
#include <string>
#include "object/game_object.h"
#include "object/system_object.h"
#include "object/3d_object.h"
#include "object/2d_object.h"


// もともとはSceneを継承していたが継承する意味がよくわからなかったので外した

class State
{
public:
	// stateを用いる各sceneにmanager的なものを作成してもらうor仲介するクラスを作成する
	State() = default;
	State(State&& other) noexcept = default;
	// 
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	template<SupportedGameObject T> // Object2DかObject3Dを継承した型のみ許可->謎にGameObjectだとエラーでないのを回避できる
	T* AddGameObject(int layerNum, Transform trans = Transform())
	{
		// 中で型を比べる
		if constexpr (!std::is_base_of_v<Object3D, T> && !std::is_base_of_v<Object2D, T>)
		{
			static_assert(std::is_base_of_v<Object3D, T> || std::is_base_of_v<Object2D, T>, "Unsupported GameObject type");
			return nullptr; // 型が違う場合はnullptrを返す
		}
		// 初期化処理実行
		std::unique_ptr<T> game_obj = std::make_unique<T>();
		// 初期化に引数ありの場合はreturnから実行する形にかな
		game_obj->Init(trans);

		if constexpr (std::is_base_of_v<Object2D, T>)
		{
			// 2Dオブジェクトの場合

			// layerNumとコンテナのサイズを比べる
			if (layerNum < 0)
			{
				// 
			}
			else if (layerNum >= static_cast<int>(m_Objects2D.size()))
			{
				// layerNumがコンテナのサイズ以上ならその数まで空の要素を追加する
				for (int i = static_cast<int>(m_Objects2D.size()); i <= layerNum; i++)
				{
					// 追加
					m_Objects2D.emplace_back(std::list<std::unique_ptr<Object2D>>());
				}
			}
			// layerNum分iteratorを進める
			auto it = m_Objects2D.begin();
			std::advance(it, layerNum);
			// layerNumの位置に追加
			it->push_back(std::unique_ptr<Object2D>(std::move(game_obj)));
			// スマポで管理しつつも生ポインタで返すように
			return static_cast<T*>(it->back().get());
		}
		else if constexpr (std::is_base_of_v<Object3D, T>)
		{
			// 3Dオブジェクトの場合

			// layerNumとコンテナのサイズを比べる
			if (layerNum < 0)
			{
				// 
			}
			else if (layerNum >= static_cast<int>(m_Objects3D.size()))
			{
				// layerNumがコンテナのサイズ以上ならその数まで空の要素を追加する
				for (int i = static_cast<int>(m_Objects3D.size()); i <= layerNum; i++)
				{
					// 追加
					m_Objects3D.emplace_back(std::list<std::unique_ptr<Object3D>>());
				}
			}
			// layerNum分iteratorを進める
			auto it = m_Objects3D.begin();
			std::advance(it, layerNum);
			// layerNumの位置に追加
			it->push_back((std::move(game_obj)));
			// スマポで管理しつつも生ポインタで返すように
			return static_cast<T*>(it->back().get());
		}
		return nullptr; // ここに来ることはないはず
	}
	template<SystemObj T>
	//template <typename T>
	T* AddSystemObject(void)
	{
		// 生成
		std::unique_ptr<T> system_obj = std::make_unique<T>();
		// 初期化
		system_obj->Init();

		// 型の整合性はconceptで取れているのでそのままpush_back
		m_SystemObjects.push_back(std::move(system_obj));
		return static_cast<T*>(m_SystemObjects.back().get());
	}

	template <typename T>
	T* GetGameObject()
	{
		// 中で型を比べる
		if constexpr (std::is_base_of_v<Object3D, T>)
		{
			// 3Dオブジェクトの場合
			for (auto& objects3d : m_Objects3D)
			{
				for (auto& gameObject : objects3d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						return ptr; // 見つかったらポインタを返す
					}
				}
			}
		}
		else if constexpr (std::is_base_of_v<Object2D, T>)
		{
			// 2Dオブジェクトの場合
			for (auto& objects2d : m_Objects2D)
			{
				for (auto& gameObject : objects2d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						return ptr; // 見つかったらポインタを返す
					}
				}
			}
		}
		else
		{
			// 型が違う場合はエラーを出す
			static_assert(std::is_base_of_v<Object3D, T> || std::is_base_of_v<Object2D, T>, "Unsupported GameObject type");
			return nullptr; // 型が違う場合はnullptrを返す
		}
		return nullptr; // 見つからなかったらnullptrを返す
	}

	template <typename T>
	std::list<T*> GetGameObjects()
	{
		// 中で型を比べる
		if constexpr (std::is_base_of_v<Object3D, T>)
		{
			// 3Dオブジェクトの場合
			std::list<T*> result;
			for (auto& objects3d : m_Objects3D)
			{
				for (auto& gameObject : objects3d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						result.push_back(ptr); // 見つかったらリストに追加
					}
				}
			}
			return result; // 3Dオブジェクトのリストを返す
		}
		else if constexpr (std::is_base_of_v<Object2D, T>)
		{
			// 2Dオブジェクトの場合
			std::list<T*> result;
			for (auto& objects2d : m_Objects2D)
			{
				for (auto& gameObject : objects2d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						result.push_back(ptr); // 見つかったらリストに追加
					}
				}
			}
			return result; // 2Dオブジェクトのリストを返す
		}
		else
		{
			// 型が違う場合はエラーを出す
			static_assert(std::is_base_of_v<Object3D, T> || std::is_base_of_v<Object2D, T>, "Unsupported GameObject type");
			return std::list<T*>(); // 型が違う場合は空のリストを返す
		}
	}

	template <SystemObj T>
	T* GetSystemObject()
	{
		for (auto& system : m_SystemObjects)
		{
			if (auto ptr = dynamic_cast<T*>(system.get()))
			{
				return ptr;
			}
		}
		// 取れなかった場合はnullptrを返す
		return nullptr;
	}

	// タグを使ってGameObjectを取得
	GameObject* GetGameObjectByTag(const std::string& tag);
	// タグを使ってGameObjectのリストを取得
	std::list<GameObject*> GetGameObjectsByTag(const std::string& tag);

protected:
	void DeleteGameObject();
	void UpdateStateObject();
	void UpdateStateObjectByTag(const std::string& tag);
	void UpdateStateObjectByTags(const std::list<std::string>& tags);
	void DrawStateObject();
	void DrawStateObjectByTag(const std::string& tag);
	void DrawStateObjectByTags(const std::list<std::string>& tags);

private:
	std::list<std::list<std::unique_ptr<Object3D>>> m_Objects3D;
	std::list<std::list<std::unique_ptr<Object2D>>> m_Objects2D;
	std::list<std::unique_ptr<SystemObject>> m_SystemObjects; // システムオブジェクトを保存するリスト
};