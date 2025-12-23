#include "main.h"
#include "enemy/node/socket/number_socket.h"


void NumberSocket::Init(Transform trans)
{
	SetTransform(trans);
}

void NumberSocket::Uninit()
{

}

void NumberSocket::Update()
{
	// nodeがアタッチされていない時、プレイヤーがノードをドラッグして持ってる状態なら座標をチェックしつくかを試みる


	// nodeがアタッチされている時、更新処理
	// もしプレイヤーが自身の座標でクリックをしたならノードを離してプレイヤーに持たせる
}

void NumberSocket::Draw()
{
	//Renderer::Draw2D(GetTextureID(), GetPosition(), GetScale());
}

NumberNode* NumberSocket::GetNumberNode()
{
	return m_NumberNode.get();
}