#include "main.h"
#include "scene/title_scene.h"
#include "scene/initialize_scene.h"
#include "lib/renderer.h"
#include "scene/manager.h"
#include "object/ui/image.h"

#include <memory>

void InitializeScene::Init()
{
	// タイマーリセット
	m_Timer = 0;

	// 画像出す
	m_LogoImage = AddGameObject<ImageDraw>(0);
	m_LogoImage->Register(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), Vector3(256.0f, 256.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\chemical.png", false);
}

void InitializeScene::Uninit()
{
	
}

void InitializeScene::Update()
{
	// カウントインクリメント
	m_Timer++;

	Scene::UpdateObject();

	//m_LogoImage->SetRotation(Vector3(m_LogoImage->GetRotation().x + 2.0f, 0.0f, 0.0f));

	// カウントが一定値を超えたらタイトルシーンへ
	if (m_Timer > 120)
	{
		Manager::SetScene<TitleScene>();
	}
}

void InitializeScene::Draw()
{
	Scene::DrawObject();

	Scene::UpdateFinal();
}