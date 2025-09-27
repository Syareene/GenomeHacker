#pragma once

#include "main.h"

class DefaultVertex
{
public:
	static void Init();
	static void Uninit();
	static ID3D11Buffer* GetBuffer();
	static ID3D11Buffer* GetBillboardBuffer();
private:
	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11Buffer* m_BillboardVertexBuffer;
};