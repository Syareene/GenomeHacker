#include "main.h"
#include "scene/manager.h"
#include "lib/renderer.h"
#include "object/field.h"
#include "object/camera.h"
#include "player.h"
#include "lib/input.h"
#include "manager/texture_manager.h"
#include "scene/title_scene.h"
//#include "scene/initialize_scene.h"
#include "manager/shader_manager.h"
#include "manager/default_vertex.h"
#include "lib/audio.h"
#include "lib/mouse.h"
#include "lib/write_font.h"

std::unique_ptr<Scene> Manager::m_CurrentScene;
std::unique_ptr<Scene> Manager::m_NextScene = nullptr;
float Manager::m_GameSpeed = 1.0f; // ゲームの速度
std::chrono::steady_clock::time_point Manager::m_BeforeTime;
float Manager::m_DeltaTime = 0.0f; // 前回からの経過時間（ミリ秒）

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	DirectWriteCustomFont::GetInstance()->Init(Renderer::GetSwapChain());
	ShaderManager::Init();
	DefaultVertex::Init();
	Audio::InitMaster();

	// 初期シーン設定
	m_CurrentScene = std::make_unique<TitleScene>();

	// 時間初期化
	m_BeforeTime = std::chrono::steady_clock::now();
	m_DeltaTime = 0.0f;

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
	TextureManager::UnloadAllTexture();
	Renderer::Uninit();
}

void Manager::Update()
{
	// 時間取得
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> diff = now - m_BeforeTime;
	m_DeltaTime = diff.count(); // ミリ秒
	m_BeforeTime = now;

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
	Renderer::GetID2D1DeviceContext()->BeginDraw();

	m_CurrentScene->Draw();

	Renderer::GetID2D1DeviceContext()->EndDraw();
	Renderer::End();

	// nextシーンが設定されてたらシーン切り替え
	if (m_NextScene != nullptr)
	{
		// 現在のシーンを解放
		m_CurrentScene->Uninit();
		m_CurrentScene = std::move(m_NextScene);
		m_CurrentScene->Init();
		// nextシーンはnullptrに
		m_NextScene = nullptr;
	}
}
