#pragma once

// 識別用のクラス
class SystemObject
{
public:
	virtual ~SystemObject() {}
	// エラー解消用にコメントアウト
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
};