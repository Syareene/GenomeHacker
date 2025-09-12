#include "object/ui/in_game/fade.h"
#include "object/game_object.h"
#include "lib/easing.h"
#include "renderer.h"
#include <algorithm>

void Fade::Init(Transform trans)
{
	SetTransform(trans);
	// とはいえこれは画面全体フェードなのでプロパティ無視して全画面にする
	SetPosition(Vector3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2, 0.0f));
	SetScale(Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f));
	SetRotation(Vector3(0.0f, 0.0f, 0.0f));

	// シェーダー生成
	Renderer::CreateVertexShader(GetVertexShaderPointer(), GetVertexLayoutPointer(), "shader\\fadeVS.cso");
	Renderer::CreatePixelShader(GetPixelShaderPointer(), "shader\\fadePS.cso");
}

void Fade::Uninit()
{
	// fadeシェーダーは一旦ここでしか使ってないのでuninit。後に共通化するならmanagerへ
	if(GetVertexShader())
	{
		GetVertexShader()->Release();
		SetVertexShader(nullptr);
	}
	if (GetPixelShader())
	{
		GetPixelShader()->Release();
		SetPixelShader(nullptr);
	}
	if(GetVertexLayout())
	{
		GetVertexLayout()->Release();
		SetVertexLayout(nullptr);
	}
}

void Fade::Update()
{
	if(!m_IsPlaying)
	{
		// 再生していないなら更新しない
		return;
	}

	// 値に応じてアルファ値を変化(暫定コードなので本当はstateパターンで分けてswitch/ifを使わないようにすべき)
	switch (m_Power)
	{
	case FadePower::Linear:
		// 等速
		m_Alpha = GetTimeRate(static_cast<float>(m_FrameCount), static_cast<float>(m_FrameMax));
		break;
	case FadePower::Sine:
		// 一次関数
		if (m_Timing == FadeTiming::In)
		{
			m_Alpha = InSine(GetTimeRate(static_cast<float>(m_FrameCount), static_cast<float>(m_FrameMax)));
		}
		else if (m_Timing == FadeTiming::Out)
		{
			m_Alpha = OutSine(GetTimeRate(static_cast<float>(m_FrameCount), static_cast<float>(m_FrameMax)));
		}
		else if (m_Timing == FadeTiming::InOut)
		{
			m_Alpha = InOutSine(GetTimeRate(static_cast<float>(m_FrameCount), static_cast<float>(m_FrameMax)));
		}
		break;
	case FadePower::Quad:
		// 二次関数
		if(m_Timing == FadeTiming::In)
		{
			m_Alpha = InQuad(GetTimeRate(static_cast<float>(m_FrameCount), static_cast<float>(m_FrameMax)));
		}
		else if (m_Timing == FadeTiming::Out)
		{
			m_Alpha = OutQuad(GetTimeRate(static_cast<float>(m_FrameCount), static_cast<float>(m_FrameMax)));
		}
		else if (m_Timing == FadeTiming::InOut)
		{
			m_Alpha = InOutQuad(GetTimeRate(static_cast<float>(m_FrameCount), static_cast<float>(m_FrameMax)));
		}
		break;
	default:
		break;
	}

	// もし逆側から再生フラグがoffなら1-alphaにする
	if (m_IsPlayStart)
	{
		m_Alpha = 1.0f - m_Alpha;
	}

	// 出力が1.0fを超えている場合があるのでclamp
	m_Alpha = std::clamp(m_Alpha, 0.0f, 1.0f);

	// カウントインクリメント
	m_FrameCount++;

	// 最大フレーム数を超えたら終了
	if(m_FrameMax < m_FrameCount)
	{
		m_IsPlaying = false;
		m_FrameCount = 0;
	}
}

void Fade::Draw()
{
	if(!m_IsPlaying)
	{
		// 再生していないなら描画しない
		return;
	}

	// 入力レイアウト設定(シェーダーのレイアウトなのでこれもシェーダー使用するときに用意)
	Renderer::GetDeviceContext()->IASetInputLayout(GetVertexLayout());
	// シェーダー設定->とりあえずunlitだがデバッグ用のを後々用意。
	Renderer::GetDeviceContext()->VSSetShader(GetVertexShader(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(GetPixelShader(), NULL, 0);

	// 計算したalpha値を渡す
	Renderer::SetParameter(XMFLOAT4(0.0f, 0.0f, 0.0f, m_Alpha));

	// 移動、回転マトリックス設定
	GameObject::SetWorldMatrixOnDraw();

	// マテリアル設定
	GameObject::SetMaterialOnDraw();

	// 頂点バッファ設定
	GameObject::SetDefaultVertexBufferOnDraw();

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画(+1は円閉じるための描画用)
	Renderer::GetDeviceContext()->Draw(4, 0);
}

// powerはイージングの種類。0なら等速、1~6でイージングの種類
void Fade::SetFadeProperty(int frame_max, FadePower power, FadeTiming timing, bool is_play_start, bool is_play_awake)
{
	m_Timing = timing;
	m_FrameMax = frame_max;
	m_Power = power;
	m_IsPlayStart = is_play_start;
	m_IsPlaying = is_play_awake;
}
