#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer() {}
	virtual ~ConstantBuffer()
	{
		if (_cbvBuffer)
		{
			if (_cbvBuffer != nullptr)
				_cbvBuffer->Unmap(0, nullptr);

			_cbvBuffer = nullptr;
		}
	}

	void Init(CBV_REGISTER reg, uint32 size, uint32 count);
	void PushGraphicsData(void* buffer, uint32 size);
	void Clear();

private:
	void CreateBuffer();
	void CreateView();
	CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle(uint32 index);

private:
	ComPtr<ID3D12DescriptorHeap> _cbvHeap = nullptr;
	ComPtr<ID3D12Resource> _cbvBuffer = nullptr;
	BYTE* _mappedBuffer = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _cpuHandle = {};
	uint32 _handleIncrementSize = 0;

	CBV_REGISTER _reg;
	uint32 _elementSize, _elementCount;
	uint32 _currentIndex = 0;

};

