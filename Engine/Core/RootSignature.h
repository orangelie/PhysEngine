#pragma once

class RootSignature
{
public:
	RootSignature() {}
	virtual ~RootSignature() {}

	void Init();

	ComPtr<ID3D12RootSignature> GetRootSignature() const { return _rootSignature; }

private:
	array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();

private:
	ComPtr<ID3D12RootSignature> _rootSignature = nullptr;

};

