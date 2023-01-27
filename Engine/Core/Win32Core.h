#pragma once

class Win32Core
{
public:
	Win32Core(uint32 Width, uint32 Height) : _screenWidth(Width), _screenHeight(Height) {}
	virtual ~Win32Core() {}

	void Init(HINSTANCE hInstace, Engine* engine);
	static BOOL GetTearingSupport();
	static HWND GetHwnd();

	static void ToggleFullscreenWindow(IDXGISwapChain* pOutput = nullptr);
	static void GetHardwareAdapter(
			IDXGIFactory1* pFactory,
			IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter);

private:
	BOOL CheckTearingSupport();
	void RegisterWindow(Engine* engine);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static const UINT _windowStyle = WS_OVERLAPPEDWINDOW;
	static HWND _hwnd;
	static BOOL _tearingSupport;
	static bool _isFullscreen;
	static RECT _windowRect;

	HINSTANCE _hInstance = nullptr;
	uint32 _screenWidth, _screenHeight;

};

