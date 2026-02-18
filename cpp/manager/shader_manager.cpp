#include "main.h"
#include "manager/shader_manager.h"

// 影なしシェーダー
ID3D11VertexShader* ShaderManager::UnlitVertexShader = nullptr;
ID3D11PixelShader* ShaderManager::UnlitPixelShader = nullptr;
ID3D11InputLayout* ShaderManager::UnlitVertexLayout = nullptr;
// インスタンシング用シェーダー(α値描画しない)
ID3D11VertexShader* ShaderManager::InstancingVertexShader = nullptr;
ID3D11VertexShader* ShaderManager::InstancingAlphaVertexShader = nullptr;
ID3D11PixelShader* ShaderManager::InstancingPixelShader = nullptr;
ID3D11InputLayout* ShaderManager::InstancingVertexLayout = nullptr;
ID3D11InputLayout* ShaderManager::InstancingAlphaVertexLayout = nullptr;
// 影なしシェーダー(アルファ値描画しない)
ID3D11VertexShader* ShaderManager::NoAlphaVertexShader = nullptr;
ID3D11PixelShader* ShaderManager::NoAlphaPixelShader = nullptr;
ID3D11InputLayout* ShaderManager::NoAlphaVertexLayout = nullptr;
// デバッグ用シェーダー
ID3D11VertexShader* ShaderManager::DebugVertexShader = nullptr;
ID3D11PixelShader* ShaderManager::DebugPixelShader = nullptr;
ID3D11InputLayout* ShaderManager::DebugVertexLayout = nullptr;

void ShaderManager::Init()
{
	// シェーダーの初期化
	Renderer::CreateVertexShader(&UnlitVertexShader, &UnlitVertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&UnlitPixelShader, "shader\\unlitTexturePS.cso");

	Renderer::CreateVertexShader(&InstancingVertexShader, &InstancingVertexLayout, "shader\\instancingVS.cso");
	Renderer::CreateVertexShader(&InstancingAlphaVertexShader, &InstancingAlphaVertexLayout, "shader\\instancingApplyAlphaVS.cso");
	Renderer::CreatePixelShader(&InstancingPixelShader, "shader\\instancingPS.cso");

	Renderer::CreateVertexShader(&NoAlphaVertexShader, &NoAlphaVertexLayout, "shader\\unlitTextureWithNoAlphaVS.cso");
	Renderer::CreatePixelShader(&NoAlphaPixelShader, "shader\\unlitTextureWithNoAlphaPS.cso");

#ifdef _DEBUG
	Renderer::CreateVertexShader(&DebugVertexShader, &DebugVertexLayout, "shader\\debugVS.cso");
	Renderer::CreatePixelShader(&DebugPixelShader, "shader\\debugPS.cso");
# else
	DebugVertexShader = nullptr;
	DebugPixelShader = nullptr;
	DebugVertexLayout = nullptr;
#endif
}

void ShaderManager::Uninit()
{
	// シェーダーの解放
	if (UnlitVertexShader) UnlitVertexShader->Release();
	if (UnlitPixelShader) UnlitPixelShader->Release();
	if (UnlitVertexLayout) UnlitVertexLayout->Release();
	if (InstancingVertexShader) InstancingVertexShader->Release();
	if (InstancingAlphaVertexShader) InstancingAlphaVertexShader->Release();
	if (InstancingPixelShader) InstancingPixelShader->Release();
	if (InstancingAlphaVertexLayout) InstancingAlphaVertexLayout->Release();
	if (InstancingVertexLayout) InstancingVertexLayout->Release();
	if (NoAlphaVertexShader) NoAlphaVertexShader->Release();
	if (NoAlphaPixelShader) NoAlphaPixelShader->Release();
	if (NoAlphaVertexLayout) NoAlphaVertexLayout->Release();
	if (DebugVertexShader) DebugVertexShader->Release();
	if (DebugPixelShader) DebugPixelShader->Release();
	if (DebugVertexLayout) DebugVertexLayout->Release();
	UnlitVertexShader = nullptr;
	UnlitPixelShader = nullptr;
	UnlitVertexLayout = nullptr;
	InstancingVertexShader = nullptr;
	InstancingAlphaVertexShader = nullptr;
	InstancingPixelShader = nullptr;
	InstancingVertexLayout = nullptr;
	InstancingAlphaVertexLayout = nullptr;
	NoAlphaVertexShader = nullptr;
	NoAlphaPixelShader = nullptr;
	NoAlphaVertexLayout = nullptr;
	DebugVertexShader = nullptr;
	DebugPixelShader = nullptr;
	DebugVertexLayout = nullptr;
}