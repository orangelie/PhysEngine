#pragma once

#include "FontGenerator.h"

class LegacyFontManager
{
public:
	LegacyFontManager(HWND hwnd);
	virtual ~LegacyFontManager();

	void Append(const wstring& e);

	CD3DX12_CPU_DESCRIPTOR_HANDLE SrvHandle(const wstring& e) const { return _fontGenMap.find(e)->second->SrvHandle(); }

private:
	std::unordered_map<wstring, shared_ptr<FontGenerator>> _fontGenMap = {};

	HWND _hwnd = nullptr;
	HDC _hdc = nullptr;
	HDC _memDC = nullptr;

};

