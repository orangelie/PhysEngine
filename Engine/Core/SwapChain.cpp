#include "pch.h"
#include "Engine.h"
#include "SwapChain.h"
#include "Win32Core.h"
#include "FrameBuffering.h"
#include "Device.h"

void SwapChain::Init(UINT32 screenWidth, UINT32 screenHeight)
{
    IDXGIFactory4* factory = GEngine->GetFactory().Get();
#pragma region CREATE_SWAPCHAIN
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FRAME_BUFFER_COUNT;
    swapChainDesc.Width = screenWidth;
    swapChainDesc.Height = screenHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Flags = Win32Core::GetTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
    // swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    ComPtr<IDXGISwapChain1> swapChain;
    HR(factory->CreateSwapChainForHwnd(
        GEngine->GetFrameBuffering()->GetCmdQueue().Get(),
        Win32Core::GetHwnd(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain));

    if (Win32Core::GetTearingSupport())
        factory->MakeWindowAssociation(Win32Core::GetHwnd(), DXGI_MWA_NO_ALT_ENTER);

    HR(swapChain.As(&_swapChain));
#pragma endregion
}
