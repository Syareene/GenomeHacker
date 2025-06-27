#pragma once

#include "object/gameObject.h"
#include "scene/base_scene.h"
#include <memory>

class Manager
{

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void SetScene(std::unique_ptr<Scene> scene)
	{
		// 次に生成したいシーンを設定
		m_NextScene = std::move(scene);
	}
	static std::unique_ptr<Scene>& GetCurrentScene()
	{
		return m_CurrentScene;
	}
private:
	static std::unique_ptr<Scene> m_CurrentScene; // 現在のシーン
	static std::unique_ptr<Scene> m_NextScene; // 次のシーン
};