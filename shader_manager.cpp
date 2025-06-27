#include "shader_manager.h"

ID3D11VertexShader* ShaderManager::UnlitVertexShader = nullptr;
ID3D11PixelShader* ShaderManager::UnlitPixelShader = nullptr;
ID3D11InputLayout* ShaderManager::UnlitVertexLayout = nullptr;

void ShaderManager::Init()
{
	// シェーダーの初期化
	Renderer::CreateVertexShader(&UnlitVertexShader, &UnlitVertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&UnlitPixelShader, "shader\\unlitTexturePS.cso");
}

void ShaderManager::Uninit()
{
	// シェーダーの解放
	if (UnlitVertexShader) UnlitVertexShader->Release();
	if (UnlitPixelShader) UnlitPixelShader->Release();
	if (UnlitVertexLayout) UnlitVertexLayout->Release();
	UnlitVertexShader = nullptr;
	UnlitPixelShader = nullptr;
	UnlitVertexLayout = nullptr;
}