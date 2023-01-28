#pragma once

class PerlinNoiseResource
{
public:
	PerlinNoiseResource() {}
	virtual ~PerlinNoiseResource() {}

	void Init(uint64 width, uint32 height);

	CD3DX12_CPU_DESCRIPTOR_HANDLE SrvHandle() const { return _cpuHandle; }

private:
	vector<float> GenerateTexBinary(uint64 width, uint32 height, uint32 pixelSize);

private:
	ComPtr<ID3D12Resource> _tex2D = nullptr;
	ComPtr<ID3D12Resource> _uploadBuffer = nullptr;

	ComPtr<ID3D12DescriptorHeap> _descHeap = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _cpuHandle = {};

};

