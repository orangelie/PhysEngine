#include "pch.h"
#include "FrameBuffering.h"
#include "SwapChain.h"
#include "Engine.h"
#include "Shader.h"
#include "Mesh.h"
#include "RootSignature.h"
#include "Win32Core.h"
#include "SceneManager.h"

const Resolution FrameBuffering::_resolutionOptions[] =
{
	{ 800u, 600u },
	{ 1200u, 900u },
	{ 1280u, 720u },
	{ 1920u, 1080u },
	{ 1920u, 1200u },
	{ 2560u, 1440u },
	{ 3440u, 1440u },
	{ 3840u, 2160u },
};
const UINT FrameBuffering::_resolutionOptionsCount = _countof(_resolutionOptions);

void FrameBuffering::OnSizeChanged(UINT width, UINT height, bool minimized)
{
	if ((width != _width || height != _height) && !minimized)
	{
		IDXGISwapChain3* swapChain = GEngine->GetSwapChain()->GetSwapChain().Get();
		WaitForGpu();

		for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			_renderTargetBuffer[i].Reset();
			_fenceValues[i] = _fenceValues[_frameIndex];
		}

		DXGI_SWAP_CHAIN_DESC desc = {};
		swapChain->GetDesc(&desc);
		HR(swapChain->ResizeBuffers(FRAME_BUFFER_COUNT, width, height, desc.BufferDesc.Format, desc.Flags));

		BOOL fullscreenState;
		HR(swapChain->GetFullscreenState(&fullscreenState, nullptr));
		// m_windowedMode = !fullscreenState;

		_frameIndex = swapChain->GetCurrentBackBufferIndex();

		_width = width;
		_height = height;
		_aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);

		LoadSizeDependentResources();
		_depthStencilBuffer->OnResize(_width, _height);

		FlushResourceCmdList();
	}
}

void FrameBuffering::StartCmdQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	GEngine->GetDevice().Get()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(_commandQueue.GetAddressOf()));
}

void FrameBuffering::Init()
{
	_viewport = { 0, 0, static_cast<FLOAT>(GInitialScreenWidth), static_cast<FLOAT>(GInitialScreenHeight), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, GInitialScreenWidth, GInitialScreenHeight);
	_aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);

	ID3D12Device* device = GEngine->GetDevice().Get();

	for (int32 i = 0; i < FRAME_BUFFER_COUNT; ++i)
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_graphicsCommandAllocators[i].GetAddressOf()));
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_rscCommandAllocator.GetAddressOf()));
	
	_frameIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _graphicsCommandAllocators[_frameIndex].Get(),
		nullptr, IID_PPV_ARGS(_graphicsCommandList.GetAddressOf()));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _rscCommandAllocator.Get(),
		nullptr, IID_PPV_ARGS(_rscCommandList.GetAddressOf()));
	// _rscCommandList->Close();
	_graphicsCommandList->Close();
	
	device->CreateFence(_fenceValues[_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.GetAddressOf()));
	_fenceValues[_frameIndex]++;

	_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	WaitForGpu();


	_rtvIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = FRAME_BUFFER_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HR(DEVICE->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap)));

	static const uint32 descriptor_count = 32;
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (uint32 i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		for (uint32 k = 0; k < (uint32)CONSTANT_BUFFER_INDEX::COUNT; ++k)
			_constantBuffers[i].push_back(make_shared<ConstantBuffer>());
		_constantBuffers[i][(uint32)CONSTANT_BUFFER_INDEX::Transform]->Init(CBV_REGISTER::b0, sizeof(TransformPass), descriptor_count);
		_constantBuffers[i][(uint32)CONSTANT_BUFFER_INDEX::Test]->Init(CBV_REGISTER::b1, sizeof(float) * 4, descriptor_count);

		_descriptorTables[i] = make_shared<DescriptorTable>();
		_descriptorTables[i]->Init(descriptor_count);

		GEngine->GetSwapChain()->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(_renderTargetBuffer[i].GetAddressOf()));
		DEVICE->CreateRenderTargetView(_renderTargetBuffer[i].Get(), nullptr, cpuHandle);
		cpuHandle.ptr += _rtvIncrementSize;
	}

	_depthStencilBuffer->Init(_width, _height);
	_perlinNoiseResource->Init(_width, _height);

	SceneManager::GetInstance()->Init();
}

void FrameBuffering::FlushResourceCmdList()
{
	_rscCommandList->Close();
	ID3D12CommandList* cmdLists[] = { _rscCommandList.Get() };
	_commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	MoveToNextFrame();

	_rscCommandAllocator->Reset();
	_rscCommandList->Reset(_rscCommandAllocator.Get(), nullptr);
}

void FrameBuffering::Render(float dt)
{
	RenderBegin();

	RenderMid(dt);

	RenderEnd();
}

void FrameBuffering::RenderBegin()
{
	_graphicsCommandAllocators[_frameIndex]->Reset();
	_graphicsCommandList->Reset(_graphicsCommandAllocators[_frameIndex].Get(), nullptr);

	_graphicsCommandList->RSSetScissorRects(1, &_scissorRect);
	_graphicsCommandList->RSSetViewports(1, &_viewport);

	_constantBuffers[_frameIndex][(uint32)CONSTANT_BUFFER_INDEX::Transform]->Clear();
	_constantBuffers[_frameIndex][(uint32)CONSTANT_BUFFER_INDEX::Test]->Clear();
	_descriptorTables[_frameIndex]->Clear();
}

void FrameBuffering::RenderMid(float dt)
{
	auto signature = GEngine->GetRootSignature()->GetRootSignature().Get();
	auto transition_before = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargetBuffer[_frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	FLOAT colors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	auto dsvHandle = _depthStencilBuffer->CpuHandle();


	_graphicsCommandList->ResourceBarrier(1, &transition_before);

	ID3D12DescriptorHeap* descHeaps[] = { _descriptorTables[_frameIndex]->GetDescriptorHeap().Get() };
	_graphicsCommandList->SetDescriptorHeaps(_countof(descHeaps), descHeaps);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	rtvHandle.Offset(_frameIndex, _rtvIncrementSize);
	_graphicsCommandList->SetGraphicsRootSignature(signature);
	_graphicsCommandList->ClearRenderTargetView(rtvHandle, colors, 0, nullptr);
	_graphicsCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	_graphicsCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	SceneManager::GetInstance()->Update("TestScene");
}

void FrameBuffering::RenderEnd()
{
	auto transition_after = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargetBuffer[_frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	_graphicsCommandList->ResourceBarrier(1, &transition_after);

	_graphicsCommandList->Close();
	ID3D12CommandList* cmdLists[] = { _graphicsCommandList.Get() };
	_commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	UINT presentFlags = (GEngine->GetWin32Core()->GetTearingSupport()) ? DXGI_PRESENT_ALLOW_TEARING : 0;
	HR(GEngine->GetSwapChain()->GetSwapChain()->Present(0, presentFlags));

	MoveToNextFrame();
}

void FrameBuffering::WaitForGpu()
{
	HR(_commandQueue->Signal(_fence.Get(), _fenceValues[_frameIndex]));

	HR(_fence->SetEventOnCompletion(_fenceValues[_frameIndex], _fenceEvent));
	WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);

	_fenceValues[_frameIndex]++;
}

void FrameBuffering::MoveToNextFrame()
{
	const UINT64 currentFenceValue = _fenceValues[_frameIndex];
	HR(_commandQueue->Signal(_fence.Get(), currentFenceValue));

	_frameIndex = GEngine->GetSwapChain()->GetBackBufferIndex();

	if (_fence->GetCompletedValue() < _fenceValues[_frameIndex])
	{
		HR(_fence->SetEventOnCompletion(_fenceValues[_frameIndex], _fenceEvent));
		WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);
	}

	_fenceValues[_frameIndex] = currentFenceValue + 1;
}

void FrameBuffering::UpdateRectAndScissor()
{
	_viewport = { 0, 0, static_cast<FLOAT>(_width), static_cast<FLOAT>(_height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, _width, _height);
	/*
	float viewWidthRatio = static_cast<float>(_resolutionOptions[_resolutionIndex].width) / _width;
	float viewHeightRatio = static_cast<float>(_resolutionOptions[_resolutionIndex].height) / _height;

	float x = 1.0f;
	float y = 1.0f;

	if (viewWidthRatio < viewHeightRatio)
		x = viewWidthRatio / viewHeightRatio;
	else
		y = viewHeightRatio / viewWidthRatio;

	_viewport.TopLeftX = _width * (1.0f - x) / 2.0f;
	_viewport.TopLeftY = _height * (1.0f - y) / 2.0f;
	_viewport.Width = x * _width;
	_viewport.Height = y * _height;

	_scissorRect.left = static_cast<LONG>(_viewport.TopLeftX);
	_scissorRect.right = static_cast<LONG>(_viewport.TopLeftX + _viewport.Width);
	_scissorRect.top = static_cast<LONG>(_viewport.TopLeftY);
	_scissorRect.bottom = static_cast<LONG>(_viewport.TopLeftY + _viewport.Height);
	*/
}

void FrameBuffering::LoadSizeDependentResources()
{
	UpdateRectAndScissor();

	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (uint32 i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		GEngine->GetSwapChain()->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(_renderTargetBuffer[i].GetAddressOf()));
		DEVICE->CreateRenderTargetView(_renderTargetBuffer[i].Get(), nullptr, cpuHandle);
		cpuHandle.Offset(1, _rtvIncrementSize);
	}
}
