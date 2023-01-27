#pragma once

class SwapChain
{
public:
	SwapChain() {}
	virtual ~SwapChain() {}

	void Init(UINT32 screenWidth, UINT32 screenHeight);

	ComPtr<IDXGISwapChain3> GetSwapChain() const { return _swapChain; }
	UINT GetBackBufferIndex() const { return _swapChain->GetCurrentBackBufferIndex(); }

private:
	ComPtr<IDXGISwapChain3> _swapChain = nullptr;

};

