#include "pch.h"
#include "Engine.h"
#include "Input.h"

void Engine::OnMouseMove(UINT x, UINT y)
{
	_mousePoint = POINT(x, y);
}

void Engine::Init(HINSTANCE hInstace)
{
	ShowCursor(FALSE);

	UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
	EnableConsole();

	ComPtr<ID3D12Debug> debugController = nullptr;
	HR(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf())));
	debugController->EnableDebugLayer();

	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	_win32Core->Init(hInstace, GEngine.get()); SetCapture(_win32Core->GetHwnd());
	_device->Init(dxgiFactoryFlags);
	_frameBuffering->StartCmdQueue();
	_swapChain->Init(GInitialScreenWidth, GInitialScreenHeight);

	_rootSignature->Init();
	_frameBuffering->Init();
	_texture->Init(L"../Resources/Textures/test.jpg");
	_textureNormal->Init(L"../Resources/Textures/test_normal.png");

	_initialCompleted = true;

	_gameTimer->Reset();
	Input::GetInstance()->Init();
}

void Engine::Render()
{
	const float dt = _gameTimer->DeltaTime();

	_frameBuffering->Render(dt);
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
	_gameTimer->Tick();
	Input::GetInstance()->Update();
}
