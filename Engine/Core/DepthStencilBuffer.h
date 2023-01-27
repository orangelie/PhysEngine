#pragma once

class DepthStencilBuffer
{
public:
	DepthStencilBuffer() {}
	virtual ~DepthStencilBuffer() {}

	void Init(uint32 width, uint32 height);
	void OnResize(uint32 width, uint32 height);

	CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle() const { return _dsvHandle; }

private:
	void CreateDSV(uint32 width, uint32 height);

private:
	ComPtr<ID3D12Resource> _dsvBuffer = nullptr;

	ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _dsvHandle = {};

};

