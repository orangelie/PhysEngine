#pragma once

#include "Win32Core.h"
#include "SwapChain.h"
#include "Device.h"
#include "FrameBuffering.h"
#include "RootSignature.h"
#include "Texture.h"
#include "GameTimer.h"
#include "FontGenerator.h"
#include "LegacyFontManager.h"

static UINT32 GInitialScreenWidth = 1080;
static UINT32 GInitialScreenHeight = 920;

class Engine
{
public:
	Engine() { }
	virtual ~Engine() { ReleaseCapture(); }
	bool InitialCompleted() const { return _initialCompleted; }

	void OnMouseMove(UINT x, UINT y);
	POINT GetMousePoint() const { return _mousePoint; }

	void Init(HINSTANCE hInstace);
	void Render();
	void RenderBegin();
	void RenderEnd();
	void Update();

	shared_ptr<Win32Core> GetWin32Core() const { return _win32Core; }
	shared_ptr<FrameBuffering> GetFrameBuffering() const { return _frameBuffering; }
	shared_ptr<SwapChain> GetSwapChain() const { return _swapChain; }
	shared_ptr<RootSignature> GetRootSignature() const { return _rootSignature; }
	shared_ptr<GameTimer> GetGameTimer() const { return _gameTimer; }

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetFontSrvHandle() const { return _legacyFontManager->SrvHandle(L"아리가또고자르"); }
	shared_ptr<Texture> GetTextureDiffuse() const { return _texture; }
	shared_ptr<Texture> GetTextureNormal() const { return _textureNormal; }
	shared_ptr<Texture> GetTextureRough() const { return _textureRough; }

	ComPtr<ID3D12Device> GetDevice() const { return _device->GetDevice(); }
	ComPtr<IDXGIFactory4> GetFactory() const { return _device->GetFactory(); }

private:
	POINT _mousePoint = {};
	bool _initialCompleted = false;

	shared_ptr<GameTimer> _gameTimer = make_shared<GameTimer>();
	shared_ptr<Win32Core> _win32Core = make_shared<Win32Core>(GInitialScreenWidth, GInitialScreenHeight);
	shared_ptr<Device> _device = make_shared<Device>();
	shared_ptr<FrameBuffering>_frameBuffering = make_shared<FrameBuffering>(GInitialScreenWidth, GInitialScreenHeight);
	shared_ptr<SwapChain> _swapChain = make_shared<SwapChain>();
	shared_ptr<RootSignature> _rootSignature = make_shared<RootSignature>();

	shared_ptr<LegacyFontManager> _legacyFontManager = nullptr;
	shared_ptr<Texture> _texture = make_shared<Texture>();
	shared_ptr<Texture> _textureNormal = make_shared<Texture>();
	shared_ptr<Texture> _textureRough = make_shared<Texture>();
};

