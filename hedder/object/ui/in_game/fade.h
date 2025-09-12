#pragma once

#include "object/2d_object.h"


// 一旦仮作成
class Fade : public Object2D
{
public:
	enum class FadeTiming
	{
		In,
		Out,
		InOut
	};
	enum class FadePower
	{
		Linear,
		Sine,
		Quad,
		//Cubic,
		//Quart,
		//Quint,
		//Expo
	};

	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	// フェード設定(最大カウント、フェードパワー、フェードがどのタイミングで強くかかるか、最初に黒く始まるかどうか、すぐに再生するか)
	void SetFadeProperty(int frame_max, FadePower power, FadeTiming timing, bool is_play_start, bool is_play_awake);
	inline void Play() { m_IsPlaying = true; m_FrameCount = 0; }
	inline bool IsPlaying() const { return m_IsPlaying; }
private:
	bool m_IsPlaying = false;
	float m_Alpha = 0.0f;
	FadeTiming m_Timing = FadeTiming::In;
	int m_FrameMax = 0;
	int m_FrameCount = 0;
	FadePower m_Power = FadePower::Linear;
	bool m_IsPlayStart = false;
};