#pragma once

#include "main.h"

class DefaultVertex
{
public:
	static void Init();
	static void Uninit();
	static ID3D11Buffer* GetBuffer();
private:
	static ID3D11Buffer* m_VertexBuffer;
};