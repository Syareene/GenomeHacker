#include "main.h"
#include "enemy/node_tab/movement.h"

void MoveTab::Init(Transform trans)
{

}

void MoveTab::Uninit()
{

}

void MoveTab::Update()
{
	auto& nodes = this->GetNodes();
	TabBase::Update();
}

void MoveTab::Draw()
{
	TabBase::Draw();
}