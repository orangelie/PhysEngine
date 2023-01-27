#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Engine.h"

void DepthStencilBuffer::Init(uint32 width, uint32 height)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_dsvHeap));
	_dsvHandle = _dsvHeap->GetCPUDescriptorHandleForHeapStart();

	CreateDSV(width, height);
}

void DepthStencilBuffer::OnResize(uint32 width, uint32 height)
{
	CreateDSV(width, height);
}

void DepthStencilBuffer::CreateDSV(uint32 width, uint32 height)
{
	CD3DX12_HEAP_PROPERTIES heapType = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC rscDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R24G8_TYPELESS, width, height); // DXGI_FORMAT_D24_UNORM_S8_UINT
	rscDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D24_UNORM_S8_UINT, 1.0f, 0);

	DEVICE->CreateCommittedResource(&heapType, D3D12_HEAP_FLAG_NONE, &rscDesc,
		D3D12_RESOURCE_STATE_COMMON, &clearValue, IID_PPV_ARGS(_dsvBuffer.GetAddressOf()));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.Texture2D.MipSlice = 0;
	DEVICE->CreateDepthStencilView(_dsvBuffer.Get(), &dsvDesc, _dsvHandle);

	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(_dsvBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	RESOURCE_CMDLIST->ResourceBarrier(1, &transition);
}
