#include "pch.h"
#include "Device.h"
#include "Win32Core.h"

void Device::Init(UINT dxgiFactoryFlags)
{
    ComPtr<IDXGIFactory4> factory;
    HR(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    if (_useWarpDevice)
    {
        ComPtr<IDXGIAdapter> warpAdapter;
        HR(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

        HR((D3D12CreateDevice(
            warpAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&_device))));
    }
    else
    {
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        Win32Core::GetHardwareAdapter(factory.Get(), &hardwareAdapter, true);

        HR((D3D12CreateDevice(
            hardwareAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&_device))));
    }

    _factory = factory;
}
