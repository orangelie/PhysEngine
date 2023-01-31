#pragma once

#include "Win32Core.h"
#include "SwapChain.h"
#include "Device.h"
#include "FrameBuffering.h"
#include "RootSignature.h"
#include "Texture.h"
#include "GameTimer.h"

static UINT32 GInitialScreenWidth = 1080;
static UINT32 GInitialScreenHeight = 920;

class Engine
{
public:
	Engine() {}
	virtual ~Engine() {}
	bool InitialCompleted() const { return _initialCompleted; }

	void Init(HINSTANCE hInstace);
	void Render();
	void RenderBegin();
	void RenderEnd();
	void Update();

	shared_ptr<Win32Core> GetWin32Core() const { return _win32Core; }
	shared_ptr<FrameBuffering> GetFrameBuffering() const { return _frameBuffering; }
	shared_ptr<SwapChain> GetSwapChain() const { return _swapChain; }
	shared_ptr<RootSignature> GetRootSignature() const { return _rootSignature; }
	shared_ptr<Texture> GetTexture() const { return _texture; }

	ComPtr<ID3D12Device> GetDevice() const { return _device->GetDevice(); }
	ComPtr<IDXGIFactory4> GetFactory() const { return _device->GetFactory(); }

private:
	bool _initialCompleted = false;

	shared_ptr<GameTimer> _gameTimer = make_shared<GameTimer>();
	shared_ptr<Win32Core> _win32Core = make_shared<Win32Core>(GInitialScreenWidth, GInitialScreenHeight);
	shared_ptr<Device> _device = make_shared<Device>();
	shared_ptr<FrameBuffering>_frameBuffering = make_shared<FrameBuffering>(GInitialScreenWidth, GInitialScreenHeight);
	shared_ptr<SwapChain> _swapChain = make_shared<SwapChain>();
	shared_ptr<RootSignature> _rootSignature = make_shared<RootSignature>();
	shared_ptr<Texture> _texture = make_shared<Texture>();

};

