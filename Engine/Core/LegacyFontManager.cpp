#include "pch.h"
#include "LegacyFontManager.h"

LegacyFontManager::LegacyFontManager(HWND hwnd)
{
	_hwnd = hwnd;
}

LegacyFontManager::~LegacyFontManager()
{
	DeleteDC(_memDC);
	DeleteDC(_hdc);
}

void LegacyFontManager::Append(const wstring& e)
{
	_hdc = GetDC(_hwnd);
	_memDC = CreateCompatibleDC(_hdc);

	shared_ptr<FontGenerator> fontGen = make_shared<FontGenerator>();
	fontGen->Init(_hwnd, _hdc, _memDC, e.c_str());
	_fontGenMap[e] = std::move(fontGen);
}
