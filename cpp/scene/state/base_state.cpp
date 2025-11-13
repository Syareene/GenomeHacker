#include "main.h"
#include "scene/state/base_state.h"

void State::Init()
{

	// override後、個別で追加したいものの追加


	//Scene::Init();
	//Scene::UpdateObjectByTag("system");
}

void State::Uninit()
{
	// override後、個別で追加したいものの追加
}

void State::Update()
{
	// override後、個別で追加したいものの追加

	// また、個別で定義してあるどのupdateを呼ぶかを指定

	// んー、これの欠点としてはscene->stateやstate->sceneの実行順序になってしまうのでレイヤーをちゃんと参照できなさそうなことか
	// まあstate変わっても常駐しているオブジェクトで常に画面の上に描画したいのってuiくらいだから一旦考えなくてもいいかも
}

void State::Draw()
{
	// updateと同じ
}