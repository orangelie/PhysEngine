#include "pch.h"
#include "Win32Core.h"
#include "Engine.h"

BOOL Win32Core::_tearingSupport = FALSE;
bool Win32Core::_isFullscreen = false;
HWND Win32Core::_hwnd = nullptr;
RECT Win32Core::_windowRect;

void Win32Core::Init(HINSTANCE hInstace, Engine* engine)
{
	_tearingSupport = CheckTearingSupport();
    _hInstance = hInstace;

    RegisterWindow(engine);
}

BOOL Win32Core::GetTearingSupport()
{
    return _tearingSupport;
}

HWND Win32Core::GetHwnd()
{
    return _hwnd;
}

BOOL Win32Core::CheckTearingSupport()
{
	BOOL bTearingSupport = FALSE;

	ComPtr<IDXGIFactory6> factory = nullptr;
	HR(CreateDXGIFactory1(IID_PPV_ARGS(factory.GetAddressOf())));
	HRESULT hResult = factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &bTearingSupport, sizeof(bTearingSupport));

	return (SUCCEEDED(hResult) && bTearingSupport);
}

void Win32Core::RegisterWindow(Engine* engine)
{
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = _hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"PhysEngine";
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, static_cast<LONG>(_screenWidth), static_cast<LONG>(_screenHeight) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    _hwnd = CreateWindow(
        windowClass.lpszClassName,
        windowClass.lpszClassName,
        _windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        _hInstance,
        engine);

    ShowWindow(_hwnd, SW_SHOW);
}

void Win32Core::GetHardwareAdapter(
    IDXGIFactory1* pFactory,
    IDXGIAdapter1** ppAdapter,
    bool requestHighPerformanceAdapter)
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;

    ComPtr<IDXGIFactory6> factory6;
    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for (
            UINT adapterIndex = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                IID_PPV_ARGS(&adapter)));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                break;
        }
    }

    if (adapter.Get() == nullptr)
    {
        for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                break;
        }
    }

    *ppAdapter = adapter.Detach();
}

void Win32Core::ToggleFullscreenWindow(IDXGISwapChain* pSwapChain)
{
    if (_isFullscreen)
    {
        SetWindowLong(_hwnd, GWL_STYLE, _windowStyle);

        SetWindowPos(
            _hwnd,
            HWND_NOTOPMOST,
            _windowRect.left,
            _windowRect.top,
            _windowRect.right - _windowRect.left,
            _windowRect.bottom - _windowRect.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);

        ShowWindow(_hwnd, SW_NORMAL);
    }
    else
    {
        // Save the old window rect so we can restore it when exiting fullscreen mode.
        GetWindowRect(_hwnd, &_windowRect);

        // Make the window borderless so that the client area can fill the screen.
        SetWindowLong(_hwnd, GWL_STYLE, _windowStyle & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

        RECT fullscreenWindowRect;
        try
        {
            if (pSwapChain)
            {
                ComPtr<IDXGIOutput> pOutput;
                HR(pSwapChain->GetContainingOutput(&pOutput));
                DXGI_OUTPUT_DESC Desc;
                HR(pOutput->GetDesc(&Desc));
                fullscreenWindowRect = Desc.DesktopCoordinates;
            }
            else
            {
                throw S_FALSE;
            }
        }
        catch (INT&)
        {
            DEVMODE devMode = {};
            devMode.dmSize = sizeof(DEVMODE);
            EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

            fullscreenWindowRect = {
                devMode.dmPosition.x,
                devMode.dmPosition.y,
                devMode.dmPosition.x + static_cast<LONG>(devMode.dmPelsWidth),
                devMode.dmPosition.y + static_cast<LONG>(devMode.dmPelsHeight)
            };
        }

        SetWindowPos(
            _hwnd,
            HWND_TOPMOST,
            fullscreenWindowRect.left,
            fullscreenWindowRect.top,
            fullscreenWindowRect.right,
            fullscreenWindowRect.bottom,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);


        ShowWindow(_hwnd, SW_MAXIMIZE);
    }

    _isFullscreen = !_isFullscreen;
}

LRESULT CALLBACK Win32Core::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Engine* engine = reinterpret_cast<Engine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CREATE:
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }
    return 0;

    case WM_KEYDOWN:
        if (wParam == VK_SPACE)
        {
            if (engine && GetTearingSupport())
            {
                ToggleFullscreenWindow(engine->GetSwapChain()->GetSwapChain().Get());
                return 0;
            }

            // engine->OnKeyDown(static_cast<UINT8>(wParam));
        }
        else if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        return 0;

    case WM_KEYUP:
        if (engine)
        {
            // engine->OnKeyUp(static_cast<UINT8>(wParam));
        }
        return 0;

    case WM_SYSKEYDOWN:
        if ((wParam == VK_RETURN) && (lParam & (1 << 29)))
        {
            if (engine && GetTearingSupport())
            {
                ToggleFullscreenWindow(engine->GetSwapChain()->GetSwapChain().Get());
                return 0;
            }
        }
        return 0;

    case WM_PAINT:
        if (engine && engine->InitialCompleted())
        {
            engine->Update();
            engine->Render();
        }
        return 0;

    case WM_SIZE:
        if (engine && engine->InitialCompleted())
        {
            RECT windowRect = {};
            GetWindowRect(hWnd, &windowRect);
            // engine->SetWindowBounds(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);

            RECT clientRect = {};
            //GetClientRect(hWnd, &clientRect);
            //engine->GetFrameBuffering()->OnSizeChanged(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, wParam == SIZE_MINIMIZED);
            engine->GetFrameBuffering()->OnSizeChanged(LOWORD(lParam), HIWORD(lParam), wParam == SIZE_MINIMIZED);
        }
        return 0;

    case WM_MOVE:
        if (engine)
        {
            RECT windowRect = {};
            GetWindowRect(hWnd, &windowRect);
            // engine->SetWindowBounds(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);

            int xPos = (int)(short)LOWORD(lParam);
            int yPos = (int)(short)HIWORD(lParam);
            // engine->OnWindowMoved(xPos, yPos);
        }
        return 0;

    case WM_DISPLAYCHANGE:
        if (engine)
        {
            // engine->OnDisplayChanged();
        }
        return 0;

    case WM_MOUSEMOVE:
        // if (engine && static_cast<UINT8>(wParam) == MK_LBUTTON)
        if (engine)
        {
            UINT x = LOWORD(lParam);
            UINT y = HIWORD(lParam);
            engine->OnMouseMove(x, y);
        }
        return 0;

    case WM_LBUTTONDOWN:
    {
        if (engine)
        {
            UINT x = LOWORD(lParam);
            UINT y = HIWORD(lParam);
            // engine->OnLeftButtonDown(x, y);
        }
    }
    return 0;

    case WM_LBUTTONUP:
    {
        if (engine)
        {
            UINT x = LOWORD(lParam);
            UINT y = HIWORD(lParam);
            // engine->OnLeftButtonUp(x, y);
        }
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
