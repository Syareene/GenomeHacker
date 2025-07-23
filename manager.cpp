#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "object/polygon.h"
#include "object/field.h"
#include "object/camera.h"
#include "player.h"
#include "input.h"
#include "enemy/enemy.h"
#include "texture_manager.h"
#include "scene/title_scene.h"
#include "shader_manager.h"
#include "default_vertex.h"
#include "lib/audio.h"
#include "lib/mouse.h"

std::unique_ptr<Scene> Manager::m_CurrentScene;
std::unique_ptr<Scene> Manager::m_NextScene = nullptr;
float Manager::m_GameSpeed = 1.0f; // ゲームの速度

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	ShaderManager::Init();
	DefaultVertex::Init();
	Audio::InitMaster();

	// 初期シーン設定
	m_CurrentScene = std::make_unique<TitleScene>();

	m_CurrentScene->Init();
}


void Manager::Uninit()
{
	// 現在のシーンを解放
	m_CurrentScene->Uninit();
	m_CurrentScene = nullptr;

	// その後各種の解放処理
	Audio::UninitMaster();
	DefaultVertex::Uninit();
	ShaderManager::Uninit();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Mouse::Update();
	Input::Update();

	m_CurrentScene->Update();


	// ガべコレ作動
	TextureManager::GarbageCollection();
	Mouse::UpdateFinal();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_CurrentScene->Draw();

	Renderer::End();

	// nextシーンが設定されてたらシーン切り替え
	if (m_NextScene != nullptr)
	{
		// 現在のシーンを解放
		m_CurrentScene->Uninit();
		m_CurrentScene = std::move(m_NextScene);
		m_CurrentScene->Init();
		// 一応nextシーンはnullptrにしておく
		m_NextScene = nullptr;
	}
}
