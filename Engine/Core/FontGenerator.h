#pragma once

class FontGenerator
{
public:
	FontGenerator();
	virtual ~FontGenerator();

	void Init(HWND hwnd, HDC hdc, HDC hMemDC, const wchar_t* szString);

	CD3DX12_CPU_DESCRIPTOR_HANDLE SrvHandle() const { return _cpuHandle; }

private:
	void save_png_memory(HBITMAP hbitmap);

	HBITMAP CreateBitmapFromText(int* width, int* height, HFONT font, const wchar_t* szString, DWORD dwLen, HBITMAP* pBitmap);
	void DDBToDIB(HDC hdc, HBITMAP hBitmap);
	vector<unsigned char> GenerateRawData(int width, int height, int texelSize);
	void CreateD3D12Resource(int width, int height);

private:
	HWND _hwnd = nullptr;
	HDC _hdc = nullptr;
	HDC _memDC = nullptr;
	BYTE* _imageBuffer = nullptr;

	ComPtr<ID3D12Resource> _tex2D = nullptr;
	ComPtr<ID3D12Resource> _uploadBuffer = nullptr;

	ComPtr<ID3D12DescriptorHeap> _descHeap = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _cpuHandle = {};

};

