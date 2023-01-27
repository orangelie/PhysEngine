#include "pch.h"
#include "Engine.h"

void Engine::Init(HINSTANCE hInstace)
{
	UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
	EnableConsole();

	ComPtr<ID3D12Debug> debugController = nullptr;
	HR(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf())));
	debugController->EnableDebugLayer();

	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	_win32Core->Init(hInstace, GEngine.get());
	_device->Init(dxgiFactoryFlags);
	_frameBuffering->StartCmdQueue();
	_swapChain->Init(GInitialScreenWidth, GInitialScreenHeight);

	_rootSignature->Init();
	_shader->Init();
	_mesh->Init();
	/*
	* ¸Þ½Ã
	*/

	_frameBuffering->Init();
	_texture->Init(L"../Resources/Textures/test.jpg");

	_initialCompleted = true;
}

void Engine::Render()
{
	_frameBuffering->Render();
}

void Engine::RenderBegin()
{
	_frameBuffering->RenderBegin();
}

void Engine::RenderEnd()
{
	_frameBuffering->RenderEnd();
}

void Engine::Update()
{

}
