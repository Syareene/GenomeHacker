#pragma once

#include <xaudio2.h>
#include "component.h"


class Audio : public Component
{
// 音量周りいじりたいなら下記見たほうがいいかも
// https://learn.microsoft.com/ja-jp/windows/win32/api/xaudio2/nn-xaudio2-ixaudio2voice

private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice{};
	BYTE*					m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};


public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Uninit();

	void Load(const char *FileName);
	void Play(bool Loop = false);
	IXAudio2SourceVoice* GetSourceVoice() const { return m_SourceVoice; }

};

