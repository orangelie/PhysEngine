#pragma once

class Device
{
public:
	Device() {}
	virtual ~Device() {}

	void Init(UINT dxgiFactoryFlags);
	ComPtr<IDXGIFactory4> GetFactory() { return _factory; }
	ComPtr<ID3D12Device> GetDevice() const { return _device; }

private:
	ComPtr<IDXGIFactory4> _factory = nullptr;
	ComPtr<ID3D12Device> _device = nullptr;
	bool _useWarpDevice = false;

};

