#pragma once

class Texture
{
public:
	Texture() {}
	virtual ~Texture() {}

	void Init(const wstring& path);

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetSrvHandle() const { return _srvHandle; }

private:
	void CreateTexture(const wstring& path);
	void CreateView();

private:
	ScratchImage _image = {};
	ComPtr<ID3D12Resource> _tex2D = nullptr;

	ComPtr<ID3D12DescriptorHeap> _srvHeap = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _srvHandle = {};

};

