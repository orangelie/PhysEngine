#pragma once

#include "ConstantBuffer.h"
#include "DescriptorTable.h"
#include "DepthStencilBuffer.h"
#include "PerlinNoiseResource.h"

static const int32 FRAME_BUFFER_COUNT = 4;

enum class CONSTANT_BUFFER_INDEX
{
	Transform,
	Test,

	COUNT,
};

class FrameBuffering
{
public:
	FrameBuffering(uint32 width, uint32 height) : _width(width), _height(height) {}
	virtual ~FrameBuffering() {}
	void OnSizeChanged(UINT width, UINT height, bool minimized);

	void StartCmdQueue();
	void Init();
	void FlushResourceCmdList();

	void Render(float dt);
	void RenderBegin();
	void RenderMid(float dt);
	void RenderEnd();

	uint32 GetClientWidth() const { return _width; }
	uint32 GetClientHeight() const { return _height; }

	ComPtr<ID3D12CommandQueue> GetCmdQueue() const { return _commandQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCurrentCmdList() const { return _graphicsCommandList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() const { return _rscCommandList; }

	vector<shared_ptr<ConstantBuffer>> GetCurrentCbvBuffer() const { return _constantBuffers[_frameIndex]; }
	shared_ptr<DescriptorTable> GetCurrentGraphicsDescTable() const { return _descriptorTables[_frameIndex]; }

private:
	void WaitForGpu();
	void MoveToNextFrame();
	void UpdateRectAndScissor();
	void LoadSizeDependentResources();

private:
	static const Resolution _resolutionOptions[];
	static const UINT _resolutionOptionsCount;
	UINT _resolutionIndex = 3;

	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	vector<shared_ptr<ConstantBuffer>> _constantBuffers[FRAME_BUFFER_COUNT] = {};
	shared_ptr<DescriptorTable> _descriptorTables[FRAME_BUFFER_COUNT] = {};
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer = make_shared<DepthStencilBuffer>();
	shared_ptr<PerlinNoiseResource> _perlinNoiseResource = make_shared<PerlinNoiseResource>();

	ComPtr<ID3D12DescriptorHeap> _rtvHeap = nullptr;
	ComPtr<ID3D12Resource> _renderTargetBuffer[FRAME_BUFFER_COUNT] = {};
	uint32 _rtvIncrementSize = 0;

	ComPtr<ID3D12CommandQueue> _commandQueue = nullptr;

	ComPtr<ID3D12CommandAllocator> _rscCommandAllocator = nullptr;
	ComPtr<ID3D12CommandAllocator> _graphicsCommandAllocators[FRAME_BUFFER_COUNT] = {};

	ComPtr<ID3D12GraphicsCommandList> _rscCommandList = nullptr;
	ComPtr<ID3D12GraphicsCommandList> _graphicsCommandList = nullptr;

	ComPtr<ID3D12Fence> _fence = nullptr;
	UINT64 _fenceValues[FRAME_BUFFER_COUNT] = {};
	UINT _frameIndex = 0;
	HANDLE _fenceEvent = nullptr;

	uint32 _width, _height;
	float _aspectRatio;

};

