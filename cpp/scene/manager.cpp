#include "main.h"
#include "scene/manager.h"
#include "lib/renderer.h"
#include "object/field.h"
#include "object/camera.h"
#include "player.h"
#include "lib/input.h"
#include "manager/texture_manager.h"
#include "scene/title_scene.h"
#include "manager/shader_manager.h"
#include "manager/default_vertex.h"
#include "lib/audio.h"
#include "lib/mouse.h"
#include "lib/write_font.h"
#include "lib/random_number.h"
// imgui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

std::unique_ptr<Scene> Manager::m_CurrentScene;
std::unique_ptr<Scene> Manager::m_NextScene = nullptr;
float Manager::m_GameSpeed = 1.0f; // ゲームの速度
std::chrono::steady_clock::time_point Manager::m_BeforeTime;
float Manager::m_DeltaTime = 0.0f; // 前回からの経過時間（ミリ秒）

void Manager::Init()
{
	Renderer::Init();

	// ImGui初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// スタイル変更
	ImGui::StyleColorsDark();
	// config
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // ドッキング有効化
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;   // Enable moving the mouse cursor

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());



	Input::Init();
	DirectWriteCustomFont::GetInstance()->Init(Renderer::GetSwapChain());
	ShaderManager::Init();
	DefaultVertex::Init();
	Audio::InitMaster();
	RandomNumber::GetInstance()->Init();

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
	// ImGui解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	Renderer::Uninit();
}

void Manager::Update()
{
	// 時間取得
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> diff = now - m_BeforeTime;
	m_DeltaTime = diff.count(); // ミリ秒
	m_BeforeTime = now;

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow(); // Show demo window! :)

	Mouse::Update();
	Input::Update();

	m_CurrentScene->Update();


	// ガべコレ作動
	TextureManager::GarbageCollection();
	Mouse::UpdateFinal();
}

void Manager::Draw()
{
	// レンダリング開始
	Renderer::Begin();
	Renderer::GetID2D1DeviceContext()->BeginDraw();
	// メイン描画
	m_CurrentScene->Draw();
	// D2D描画確定
	Renderer::GetID2D1DeviceContext()->EndDraw();

	// 描画データ作成
	ImGui::Render();
	// ImGuiの描画
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// 描画確定
	Renderer::End();

	ImGuiIO& io = ImGui::GetIO();
	// ウィンドウが外に出ている場合の処理
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

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
