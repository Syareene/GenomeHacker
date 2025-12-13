#pragma once

#include "scene/base_scene.h"
#include <memory>
#include <chrono>

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

	static float m_DeltaTime; // 前回からの経過時間（ミリ秒）

private:
	static std::unique_ptr<Scene> m_CurrentScene; // 現在のシーン
	static std::unique_ptr<Scene> m_NextScene; // 次のシーン
	static float m_GameSpeed; // ゲームの速度
	static std::chrono::steady_clock::time_point m_BeforeTime; // 前回時刻
};