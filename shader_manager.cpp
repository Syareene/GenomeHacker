#include "shader_manager.h"

ID3D11VertexShader* ShaderManager::UnlitVertexShader = nullptr;
ID3D11PixelShader* ShaderManager::UnlitPixelShader = nullptr;
ID3D11InputLayout* ShaderManager::UnlitVertexLayout = nullptr;
ID3D11VertexShader* ShaderManager::NoAlphaVertexShader = nullptr;
ID3D11PixelShader* ShaderManager::NoAlphaPixelShader = nullptr;
ID3D11InputLayout* ShaderManager::NoAlphaVertexLayout = nullptr;

void ShaderManager::Init()
{
	// シェーダーの初期化
	Renderer::CreateVertexShader(&UnlitVertexShader, &UnlitVertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&UnlitPixelShader, "shader\\unlitTexturePS.cso");

	Renderer::CreateVertexShader(&NoAlphaVertexShader, &NoAlphaVertexLayout, "shader\\unlitTextureWithNoAlphaVS.cso");
	Renderer::CreatePixelShader(&NoAlphaPixelShader, "shader\\unlitTextureWithNoAlphaPS.cso");
}

void ShaderManager::Uninit()
{
	// シェーダーの解放
	if (UnlitVertexShader) UnlitVertexShader->Release();
	if (UnlitPixelShader) UnlitPixelShader->Release();
	if (UnlitVertexLayout) UnlitVertexLayout->Release();
	if (NoAlphaVertexShader) NoAlphaVertexShader->Release();
	if (NoAlphaPixelShader) NoAlphaPixelShader->Release();
	if (NoAlphaVertexLayout) NoAlphaVertexLayout->Release();
	UnlitVertexShader = nullptr;
	UnlitPixelShader = nullptr;
	UnlitVertexLayout = nullptr;
	NoAlphaVertexShader = nullptr;
	NoAlphaPixelShader = nullptr;
	NoAlphaVertexLayout = nullptr;
}