#include "pch.h"
#include "FontGenerator.h"
#include "Engine.h"

FontGenerator::FontGenerator()
{
}

FontGenerator::~FontGenerator()
{
}

void FontGenerator::Init(HWND hwnd, HDC hdc, HDC hMemDC, const wchar_t* szString)
{
	_hwnd = hwnd;
	_hdc = hdc;
	_memDC = hMemDC;

	int width = 0;
	int height = 0;
	HFONT hFont = CreateFontW(
		15,                  // 문자 폭
		0,                   // 문자 높이
		0,                   // 문자 기울기
		0,                   // 문자 방향
		FW_NORMAL,           // 문자 굵기
		FALSE,               // 기울기
		FALSE,               // 밑줄
		0,                   // 취소선
		DEFAULT_CHARSET,     // 문자셋
		OUT_DEFAULT_PRECIS,  // 출력 정확도
		CLIP_DEFAULT_PRECIS, // 클리핑 정확도
		DEFAULT_QUALITY,     // 출력의 질
		DEFAULT_PITCH | FF_SWISS, L"Arial");
	HBITMAP oldBitmap = nullptr;

	HBITMAP hBitmap = CreateBitmapFromText(&width, &height, hFont, szString, (DWORD)wcslen(szString), &oldBitmap);
	DDBToDIB(_memDC, hBitmap);

#if defined(DEBUG) | defined(_DEBUG)
	save_png_memory(hBitmap);
#endif

	CreateD3D12Resource(width, height);

	SelectObject(_memDC, oldBitmap);
	DeleteObject(hBitmap);
	delete[] _imageBuffer;
}

void FontGenerator::DDBToDIB(HDC hdc, HBITMAP hBitmap)
{
	BITMAP bitmap = {};
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	BITMAPINFOHEADER bih = {};
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biWidth = bitmap.bmWidth;
	bih.biHeight = bitmap.bmHeight;
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = (bih.biWidth << 2) * bih.biHeight;
	
	BITMAPINFO bmi = {};
	bmi.bmiHeader = bih;

	_imageBuffer = new BYTE[bih.biSizeImage];
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, _imageBuffer, &bmi, DIB_RGB_COLORS);
}

void FontGenerator::save_png_memory(HBITMAP hBitmap)
{
	CoInitialize(NULL);

	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput tmp;
	Gdiplus::GdiplusStartup(&token, &tmp, NULL);

	Gdiplus::Bitmap bmp(hBitmap, nullptr);
	
	//write to IStream
	IStream* istream = nullptr;
	if (CreateStreamOnHGlobal(NULL, TRUE, &istream) != 0)
		return;

	CLSID clsid_png;
	if (CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &clsid_png) != 0)
		return;
	Gdiplus::Status status = bmp.Save(istream, &clsid_png);
	if (status != Gdiplus::Status::Ok)
		return;

	//get memory handle associated with istream
	HGLOBAL hg = NULL;
	if (GetHGlobalFromStream(istream, &hg) != S_OK)
		return;

	//copy IStream to buffer
	size_t bufsize = GlobalSize(hg);
	std::vector<BYTE> data(bufsize);
	data.resize(bufsize);

	//lock & unlock memory
	LPVOID pimage = GlobalLock(hg);
	if (!pimage)
		return;
	memcpy(&data[0], pimage, bufsize);
	GlobalUnlock(hg);

	istream->Release();

	ofstream ofs("output_test_font.png", std::ios::binary);
	ofs.write((char*)data.data(), data.size());
	ofs.close();
}

HBITMAP FontGenerator::CreateBitmapFromText(int* width, int* height, HFONT font, const wchar_t* szString, DWORD dwLen, HBITMAP* pBitmap)
{
	HBITMAP hBitmap = nullptr;
	HBITMAP hOldBitmap = nullptr;
	RECT rect = {};
	SIZE fSize = {};
	int fWidth = 0, fHeight = 0;

	HFONT oldFont = (HFONT)SelectObject(_hdc, font);
	BOOL bGetSize = GetTextExtentPoint32W(_hdc, szString, dwLen, &fSize);
	SelectObject(_hdc, oldFont);

	if (!bGetSize)
		goto lb_return;

	fWidth = fSize.cx; fWidth = ((fWidth + 0x00000001) & 0xfffffffe);
	fHeight = fSize.cy;

	rect.left = 0;
	rect.top = 0;
	rect.right = fWidth;
	rect.bottom = fHeight;

	hBitmap = CreateCompatibleBitmap(_hdc, fWidth, fHeight);
	oldFont = (HFONT)SelectObject(_memDC, font);

	SetTextColor(_memDC, RGB(255, 0, 255));
	SetBkColor(_memDC, RGB(0, 0, 0));
	SetBkMode(_memDC, OPAQUE);

	hOldBitmap = (HBITMAP)SelectObject(_memDC, hBitmap);
	FillRect(_memDC, &rect, CreateSolidBrush(0x00000000));

	DrawTextW(_memDC, szString, dwLen, &rect, DT_SINGLELINE | DT_NOPREFIX | DT_BOTTOM);

	SelectObject(_memDC, oldFont);

	*pBitmap = hOldBitmap;
	*width = fWidth;
	*height = fHeight;

lb_return:
	return hBitmap;
}

vector<unsigned char> FontGenerator::GenerateRawData(int width, int height, int texelSize)
{
	uint32 textureSize = (texelSize * width) * height;
	std::vector<unsigned char> data(textureSize);
	unsigned char* pData = &data[0];

	/*
	for (int i = 0; i < textureSize; i += 4)
	{
		pData[i + 0] = _imageBuffer[i + 0];
		pData[i + 1] = _imageBuffer[i + 1];
		pData[i + 2] = _imageBuffer[i + 2];
		pData[i + 3] = 0;
	}
	*/

	const int n_width = width * texelSize;
	const int height_1 = height - 1;
	for (int i = 0; i < height; ++i)
	{
		int r_height = height_1 - i;
		for (int j = 0; j < n_width; j += 4)
		{
			pData[i * n_width + j + 0] = _imageBuffer[r_height * n_width + j + 0];
			pData[i * n_width + j + 1] = _imageBuffer[r_height * n_width + j + 1];
			pData[i * n_width + j + 2] = _imageBuffer[r_height * n_width + j + 2];
			pData[i * n_width + j + 3] = 0;
		}
	}

	return data;
}

void FontGenerator::CreateD3D12Resource(int width, int height)
{
	DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
	const uint32 texelSize = 4; // B8G8R8A8

	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	CD3DX12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	DEVICE->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(_tex2D.GetAddressOf()));

	const uint64 bufferSize = ::GetRequiredIntermediateSize(_tex2D.Get(), 0, 1);
	desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	DEVICE->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_uploadBuffer.GetAddressOf()));

	vector<unsigned char> data = GenerateRawData(width, height, texelSize);

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
