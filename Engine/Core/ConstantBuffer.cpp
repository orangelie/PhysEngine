#include "pch.h"
#include "ConstantBuffer.h"
#include "DescriptorTable.h"
#include "FrameBuffering.h"
#include "Engine.h"

void ConstantBuffer::Init(CBV_REGISTER reg, uint32 size, uint32 count)
{
	_reg = reg;
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
	CreateView();
}

void ConstantBuffer::PushGraphicsData(void* buffer, uint32 size)
{
	assert(_currentIndex < _elementCount);
	assert(_elementSize == ((size + 255) & ~255));

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = CpuHandle(_currentIndex);
	FRAME_BUFFERING->GetCurrentGraphicsDescTable()->SetCBV(cpuHandle, _reg);

	_currentIndex++;
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

void ConstantBuffer::CreateBuffer()
{
	const uint32 bufferSize = _elementSize * _elementCount;
	CD3DX12_RESOURCE_DESC rscDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	CD3DX12_HEAP_PROPERTIES heapType = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	DEVICE->CreateCommittedResource(&heapType, D3D12_HEAP_FLAG_NONE, &rscDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_cbvBuffer.GetAddressOf()));

	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
}

void ConstantBuffer::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = _elementCount;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_cbvHeap.GetAddressOf()));

	_cpuHandle = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32 i = 0; i < _elementCount; ++i)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CpuHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.SizeInBytes = _elementSize;
		cbvDesc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress();

		DEVICE->CreateConstantBufferView(&cbvDesc, handle);
	}
}

CD3DX12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::CpuHandle(uint32 index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandle, index * _handleIncrementSize);
}
