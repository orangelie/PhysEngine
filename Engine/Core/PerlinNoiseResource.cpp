#include "pch.h"
#include "PerlinNoiseResource.h"
#include "PerlinNoise.h"
#include "Engine.h"

void PerlinNoiseResource::Init(uint64 width, uint32 height)
{
	DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
	const uint32 texelSize = 3; // R32G32B32

	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	CD3DX12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	DEVICE->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(_tex2D.GetAddressOf()));

	const uint64 bufferSize = ::GetRequiredIntermediateSize(_tex2D.Get(), 0, 1);
	desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	DEVICE->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_uploadBuffer.GetAddressOf()));

	vector<float> data = GenerateTexBinary(width, height, texelSize);

	D3D12_SUBRESOURCE_DATA subResoruce = {};
	subResoruce.pData = data.data();
	subResoruce.RowPitch = width * texelSize;
	subResoruce.SlicePitch = subResoruce.RowPitch * height;

	CD3DX12_RESOURCE_BARRIER rscBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_tex2D.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	UpdateSubresources(RESOURCE_CMDLIST.Get(), _tex2D.Get(), _uploadBuffer.Get(), 0, 0, 1, &subResoruce);
	RESOURCE_CMDLIST->ResourceBarrier(1, &rscBarrier);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_descHeap.GetAddressOf()));
	_cpuHandle = _descHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	DEVICE->CreateShaderResourceView(_tex2D.Get(), &srvDesc, _cpuHandle);
}

vector<float> PerlinNoiseResource::GenerateTexBinary(uint64 width, uint32 height, uint32 pixelSize)
{
	siv::PerlinNoise perlin{ std::random_device{} };

	const uint32 rowPitch = static_cast<uint32>(width) * pixelSize;
	const uint32 cellPitch = rowPitch >> 3;	// The width of a cell in the checkboard texture.
	const uint32 cellHeight = static_cast<uint32>(width) >> 3;	// The height of a cell in the checkerboard texture.
	const uint32 textureSize = rowPitch * height;

	std::vector<float> data(textureSize);
	float* pData = &data[0];
	
	uint32 o = 0, p = 0;
	for (uint32 n = 0; n < textureSize; n += pixelSize)
	{
		uint32 x = n % rowPitch;
		uint32 y = n / rowPitch;
		uint32 i = x / cellPitch;
		uint32 j = y / cellHeight;

		pData[n + 0] = std::fabs((float)perlin.noise2D((double)o, (double)p));	// R
		pData[n + 1] = std::fabs((float)perlin.noise2D((double)o, (double)p));	// G
		pData[n + 2] = std::fabs((float)perlin.noise2D((double)o, (double)p));	// B

		o++;
		if (o == width - 1)
		{
			o = 0;
			p++;
		}
			
	}

	/*
	uint32 n = 0;
	for (uint32 i = 0; i < height; ++i)
	{
		for (uint32 j = 0; j < width; ++j)
		{
			float d = std::fabs((float)perlin.noise2D((double)j, (double)i));
			pData[n] = d;
			++n;
		}
	}
	*/

	return data;
}
