#pragma once

#include "scene/base_scene.h"
#include <memory>

// エラー解消用に前方宣言
class Scene;

class Manager
{

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	template <typename T>
	static inline void SetScene()
	{
		// 次に生成したいシーンを設定
		m_NextScene = std::make_unique<T>();
	}
	static inline std::unique_ptr<Scene>& GetCurrentScene()
	{
		return m_CurrentScene;
	}
	static inline void SetGameSpeed(const float& speed)
	{
		m_GameSpeed = speed;
	}

	const static inline float GetGameSpeed()
	{
		return m_GameSpeed;
	}

private:
	static std::unique_ptr<Scene> m_CurrentScene; // 現在のシーン
	static std::unique_ptr<Scene> m_NextScene; // 次のシーン
	static float m_GameSpeed; // ゲームの速度
};