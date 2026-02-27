#pragma once

#include "object/2d_object.h"

class UI : public Object2D
{
public:
	UI() = default; // デフォルトコンストラクタ
	virtual ~UI() {}
	UI(UI&& Other) noexcept
		: Object2D(std::move(Other))
		, m_NoUpdate(Other.m_NoUpdate)
	{
	}
	UI& operator=(UI&& Other) noexcept
	{
		if (this != &Other)
		{
			Object2D::operator=(std::move(Other));
			m_NoUpdate = Other.m_NoUpdate;
		}
		return *this;
	}
	void Init(Transform trans = Transform())
	{
		SetTransform(trans);
	}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
	// 更新しなくて良いオブジェクトには更新しなくていいタグを付けてコストを下げる
	
	// スライダー
	// クリック検知をして範囲内に入っていればスライドする。値を簡易的に取得できるように。
	// 基本的にサイズとか値の範囲だけ作ったら後はその値を都度返すだけの物になるので初期化子だけで良さそう。
	// 初期化子: pos/size/rot/min/max/value(初期値)/step(刻み幅)

public:
	inline void SetNoUpdate(const bool noUpdate) { m_NoUpdate = noUpdate; }
	inline const bool GetNoUpdate() const { return m_NoUpdate; }
private:
	bool m_NoUpdate = false; // 更新しないUIかどうか
};