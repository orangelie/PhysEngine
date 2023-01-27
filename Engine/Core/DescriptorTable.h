#pragma once

class DescriptorTable
{
public:
	DescriptorTable() {}
	virtual ~DescriptorTable() {}

	void Init(uint32 count);

	void Clear();
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);

	void CommitTable();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 reg);

private:
	ComPtr<ID3D12DescriptorHeap> _descHeap = nullptr;
	uint32 _groupCount, _handleSize, _groupSize, _currentGroupIndex = 0;

};

