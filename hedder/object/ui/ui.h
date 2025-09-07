#pragma once

#include "object/2d_object.h"

class UI : public Object2D
{
public:
	virtual ~UI() { OutputDebugStringA("~UI\n"); }
	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
	// 更新しなくて良いオブジェクトには更新しなくていいタグを付けてコストを下げる

	// クリック検知

	// 画像表示
	// 都度描画するだけなので初期化子だけで良い
	// 初期化子: pos/size/rot/texture
	// 完成した!
	
	// text
	// jsonとかから読み込んで表示。翻訳とかもそのうちできるかもねぇ。
	// 後はテキスト表示機構を作らなければならない
	// 初期化子: pos/rot/fontsize/text(将来的にtextのファイルになるかも)/(プロパティ:太字とか?)

	// ボタン
	// クリック検知をして、範囲内に入っていれば処理を実行する->基本的にはボタン毎にクラス作成になるのかなぁ?
	// unityみたいなインスペクター内から初期化子にいれるには情報量多いしなぁ、、
	// 各クラスで作ることになるので初期化子はなしかな。ただ実行先のクラスをうまいこと作れれば初期化子から作れる。
	// 完成した!
	
	// スライダー
	// クリック検知をして範囲内に入っていればスライドする。値を簡易的に取得できるように。
	// 基本的にサイズとか値の範囲だけ作ったら後はその値を都度返すだけの物になるので初期化子だけで良さそう。
	// 初期化子: pos/size/rot/min/max/value(初期値)/step(刻み幅)

	// もし子objとかを作るなら初期化子の座標に元objの座標を参照するとかで表現する形にしようかなぁ。


	// 各派生クラスを作る

public:
	void SetNoUpdate(bool noUpdate) { m_NoUpdate = noUpdate; }
	bool GetNoUpdate() const { return m_NoUpdate; }
private:
	bool m_NoUpdate = false; // 更新しないUIかどうか
};