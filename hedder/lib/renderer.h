#pragma once


#include "d2d1_1.h"


struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};



struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	XMFLOAT4	Direction;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Ambient;
};



class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device* m_Device;
	static ID3D11DeviceContext* m_DeviceContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer* m_WorldBuffer;
	static ID3D11Buffer* m_ViewBuffer;
	static ID3D11Buffer* m_ProjectionBuffer;
	static ID3D11Buffer* m_MaterialBuffer;
	static ID3D11Buffer* m_LightBuffer;
	static ID3D11Buffer* m_ParameterBuffer;
	static ID3D11Buffer* m_CameraBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState* m_BlendState;
	static ID3D11BlendState* m_BlendStateAlpha;
	static ID3D11BlendState* m_BlendStateATC;

	static ID2D1Factory1* m_D2DFactory;
	static ID2D1Device* m_D2DDevice;
	static ID2D1DeviceContext* m_D2DDeviceContext;
	static ID2D1Bitmap1* m_D2DTargetBitmap;

public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(XMMATRIX WorldMatrix);
	static void SetViewMatrix(XMMATRIX ViewMatrix);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetParameter(XMFLOAT4 parameter);
	static void SetCameraPosition(XMFLOAT3 position);

	static void Draw2D();

	static ID3D11Device* GetDevice(void) { return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }
	static IDXGISwapChain* GetSwapChain(void) { return m_SwapChain; }
	// 加算合成用
	static ID3D11BlendState* GetBlendState(void) { return m_BlendState; }
	// 通常描画用(alpha込)
	static ID3D11BlendState* GetBlendStateAlpha(void) { return m_BlendStateAlpha; }

	static ID2D1Factory1* GetID2D1Factory(void) { return m_D2DFactory; }
	static ID2D1Device* GetID2D1Device(void) { return m_D2DDevice; }
	static ID2D1DeviceContext* GetID2D1DeviceContext(void) { return m_D2DDeviceContext; }
	static ID2D1Bitmap1* GetID2D1Bitmap(void) { return m_D2DTargetBitmap; }


	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
};