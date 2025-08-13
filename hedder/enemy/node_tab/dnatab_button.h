#pragma once

#include "object/ui/button.h"

class DNAButton : public Button
{
	// これ正確にはbuttonクラス継承とは違うんだよな、中身の関数がほしいだけだから。
	// と思ったが現状のシステム考えるとそうでもない気がしてきた。->クリック先のfuncを作る必要あり

public:
	// ゲーム側からdnaタブに行くときの処理
	void ToDNATab();
	// dnaタブからゲーム側に行く処理
	void ToGame();
};